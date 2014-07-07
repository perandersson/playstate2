#include <core/Memory.h>
#include "DeferredRenderPipeline.h"

DeferredRenderPipeline::DeferredRenderPipeline()
: mFullscreenQuad(nullptr), mSphere(nullptr),
mDiffuseRenderTarget(nullptr), mNormalsRenderTarget(nullptr),
mDepthRenderTarget(nullptr), mLightRenderTarget(nullptr), mCubeShadowMap(nullptr)
{
	ActiveWindow::AddWindowResizedListener(this);

	const Size& windowSize = ActiveWindow::GetSize();

	mDiffuseRenderTarget = RenderContext::CreateRenderTarget2D(windowSize, TextureFormat::RGBA16F);
	//mMaterialsRenderTarget = RenderContext::CreateRenderTarget2D(windowSize, TextureFormat::RGBA16F);
	mNormalsRenderTarget = RenderContext::CreateRenderTarget2D(windowSize, TextureFormat::RGBA16F);
	mDepthRenderTarget = RenderContext::CreateRenderTarget2D(windowSize, TextureFormat::DEPTH32F);
	mLightRenderTarget = RenderContext::CreateRenderTarget2D(windowSize, TextureFormat::RGBA);

	mCubeShadowMap = RenderContext::CreateRenderTargetCube(Size(256, 256), TextureFormat::DEPTH32F);

	mDeferredEffect = ResourceManager::GetResource<Effect>("/demo/effects/deferred/geometry.effect");
	mPointLightEffect = ResourceManager::GetResource<Effect>("/demo/effects/deferred/pointlight.effect");
	//mSpotLightEffect = ResourceManager::GetResource<Effect>("/demo/effects/deferred/spotlight.effect");
	mTexturedEffect = ResourceManager::GetResource<Effect>("/demo/effects/deferred/result.effect");
	mDebugEffect = ResourceManager::GetResource<Effect>("/demo/effects/debug.effect");
	mWhiteTexture = ResourceManager::GetResource<Texture2D>("/engine/textures/white.png");

	//mUniformSphere = VertexBufferUtils::CreateSphere(1, 0, 0, 0, BufferUsage::STATIC);
	mSphere = Sphere::Create(1, 10, BufferUsage::STATIC);
	//const Vector3 position(0.0f, 10.0f, 0.0f);
	//const Vector3 direction(0.0f, -1.0f, 0.0f);
	//const float cutoff = 45.0f;
	//mCone = VertexBufferUtils::CreateSpotLight(position, direction, cutoff);
	mFullscreenQuad = VertexBufferUtils::CreateFullscreenQuad();
}

DeferredRenderPipeline::~DeferredRenderPipeline()
{
	if (mSphere != nullptr) {
		delete mSphere;
		mSphere = nullptr;
	}

	//if (mCone != nullptr) {
	//	delete mCone;
	//	mCone = nullptr;
	//}

	if (mFullscreenQuad != nullptr) {
		delete mFullscreenQuad;
		mFullscreenQuad = nullptr;
	}

	delete mDiffuseRenderTarget;
	delete mNormalsRenderTarget;
	delete mDepthRenderTarget;
	delete mLightRenderTarget;

	mDiffuseRenderTarget = nullptr;
	mDepthRenderTarget = nullptr;
	mLightRenderTarget = nullptr;
	mNormalsRenderTarget = nullptr;

	delete mCubeShadowMap;
	mCubeShadowMap = nullptr;

	ActiveWindow::RemoveWindowResizedListener(this);
}

void DeferredRenderPipeline::Render(const Scene& scene, const Camera* camera)
{
	FindQuery query = { camera, RenderableFilter::DEFAULT };
	// Submit the rendering of the scenes vertex components to another thread.
	// This is not how it should be in the end. Purpose for this is to verify that
	// threaded rendering works as intended
	if (scene.Find(query, &mRenderBlockResultSet)) {
		auto fut = RenderContext::Async<bool>([this, &scene, &camera] {
			RenderState* state = RenderContext::Activate(mDeferredEffect);
			state->SetRenderTarget(mDiffuseRenderTarget, 0);
			state->SetRenderTarget(mNormalsRenderTarget, 1);
			state->SetDepthRenderTarget(mDepthRenderTarget);
			state->Clear(ClearType::COLOR | ClearType::DEPTH);

			// Set camera properties
			state->FindUniform("ProjectionMatrix")->SetMatrix(camera->GetProjectionMatrix());
			state->FindUniform("ViewMatrix")->SetMatrix(camera->GetViewMatrix());
			state->FindUniform("FarClip")->SetFloat(camera->GetFarPlane());

			auto modelMatrix = state->FindUniform("ModelMatrix");
			auto diffuseTexture = state->FindUniform("DiffuseTexture");
			auto diffuseColor = state->FindUniform("DiffuseColor");

			RenderBlockResultSet::Iterator it = mRenderBlockResultSet.GetIterator();
			RenderBlockResultSet::Type block;
			while (block = it.Next()) {
				diffuseTexture->SetTexture(block->diffuseTexture);
				diffuseColor->SetColorRGB(block->diffuseColor);
				modelMatrix->SetMatrix(block->modelMatrix);
				state->Render(block->vertexBuffer, block->indexBuffer);
			}
			return true;
		});
		fut.get();
	}

	DrawLighting(scene, camera);
	DrawFinalResultToScreen(scene, camera);

#if defined(_DEBUG) || defined(RENDERING_TROUBLESHOOTING)
	DrawDebugInfo(scene, camera);
#endif
}

void DeferredRenderPipeline::OnWindowResized(const Size& newSize)
{
	mDiffuseRenderTarget->Resize(newSize);
	mNormalsRenderTarget->Resize(newSize);
	mDepthRenderTarget->Resize(newSize);
	mLightRenderTarget->Resize(newSize);
}

void DeferredRenderPipeline::DrawLighting(const Scene& scene, const Camera* camera)
{
	FindQuery query = { camera, RenderableFilter::DEFAULT };
	RenderState* state = RenderContext::Activate(mPointLightEffect);
	state->SetRenderTarget(mLightRenderTarget, 0);
	state->SetDepthMask(false);
	state->Clear(ClearType::COLOR);

	if (scene.Find(query, &mLightSourceResultSet)) {
		state->FindUniform("DepthTexture")->SetTexture(mDepthRenderTarget);
		state->FindUniform("NormalsTexture")->SetTexture(mNormalsRenderTarget);

		state->FindUniform("ProjectionMatrix")->SetMatrix(camera->GetProjectionMatrix());
		state->FindUniform("ViewMatrix")->SetMatrix(camera->GetViewMatrix());

		// Screen information
		state->FindUniform("ProjectionA")->SetFloat(camera->GetFarPlane() / (camera->GetFarPlane() - camera->GetNearPlane()));
		state->FindUniform("ProjectionB")->SetFloat((-camera->GetFarPlane() * camera->GetNearPlane()) / (camera->GetFarPlane() - camera->GetNearPlane()));
		const Size& size = ActiveWindow::GetSize();
		state->FindUniform("ScreenSize")->SetVector2(Vector2(size.x, size.y));
		state->FindUniform("CameraForward")->SetVector3(camera->GetLookingAtDirection());
		state->FindUniform("CameraPosition")->SetVector3(camera->GetPosition());

		state->FindUniform("LightTexture")->SetTexture(mWhiteTexture);

		IUniform* lightColor = state->FindUniform("LightColor");
		IUniform* lightPosition = state->FindUniform("LightPosition");
		IUniform* constantAttenuation = state->FindUniform("ConstantAttenuation");
		IUniform* linearAttenuation = state->FindUniform("LinearAttenuation");
		IUniform* quadraticAttenuation = state->FindUniform("QuadraticAttenuation");
		IUniform* lightRadius = state->FindUniform("LightRadius");

		LightSourceResultSet::Iterator it = mLightSourceResultSet.GetIterator();
		LightSourceResultSet::Type block;
		while (block = it.Next()) {
			// Ignore spotlights
			if (block->direction.IsNotZero())
				continue;

			lightColor->SetColorRGB(block->color);
			lightPosition->SetVector3(block->position);
			constantAttenuation->SetFloat(block->constantAttenuation);
			linearAttenuation->SetFloat(block->linearAttenuation);
			quadraticAttenuation->SetFloat(block->quadricAttenuation);
			lightRadius->SetFloat(block->radius);

			state->Render(mSphere->GetVertexBuffer(), mSphere->GetIndexBuffer());
		}

		//state = RenderContext::Activate(mSpotLightEffect);
		//state->SetRenderTarget(mLightRenderTarget, 0);
		//state->SetDepthRenderTarget(mDepthRenderTarget);

		//state->FindUniform("PositionsTexture")->SetTexture(mPositionsRenderTarget);
		//state->FindUniform("NormalsTexture")->SetTexture(mNormalsRenderTarget);
		//state->FindUniform("DepthTexture")->SetTexture(mDepthRenderTarget);

		//state->FindUniform("ProjectionMatrix")->SetMatrix(camera->GetProjectionMatrix());
		//state->FindUniform("ViewMatrix")->SetMatrix(camera->GetViewMatrix());
		//state->FindUniform("ScreenSize")->SetVector2(Vector2(size.x, size.y));

		//state->FindUniform("LightTexture")->SetTexture(mWhiteTexture);

		//lightRadius = state->FindUniform("LightRadius");
		//auto lightCutoff = state->FindUniform("LightCutoff");
		//lightColor = state->FindUniform("LightColor");
		//lightPosition = state->FindUniform("LightPosition");
		//auto lightDirection = state->FindUniform("LightDirection");

		//it = mLightSourceResultSet.GetIterator();
		//while (block = it.Next()) {
		//	// Ignore point-lights
		//	if (block->direction.IsZero())
		//		continue;

		//	lightRadius->SetFloat(block->radius);
		//	lightCutoff->SetFloat(block->radius);
		//	lightColor->SetColorRGB(block->color);
		//	lightPosition->SetVector3(block->position);
		//	lightDirection->SetVector3(block->direction);

		//	state->Render(mCone);
		//}
	}	
}

void DeferredRenderPipeline::DrawFinalResultToScreen(const Scene& scene, const Camera* camera)
{
	RenderState* state = RenderContext::Activate(mTexturedEffect);

	state->FindUniform("DiffuseTexture")->SetTexture(mDiffuseRenderTarget);
	state->FindUniform("DepthTexture")->SetTexture(mDepthRenderTarget);
	state->FindUniform("NormalsTexture")->SetTexture(mNormalsRenderTarget);
	state->FindUniform("LightTexture")->SetTexture(mLightRenderTarget);

	state->Clear(ClearType::COLOR | ClearType::DEPTH);
	state->FindUniform("AmbientColor")->SetColorRGB(scene.GetAmbientLight());
	state->FindUniform("ProjectionMatrix")->SetMatrix(Camera::GetOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f));
	state->Render(mFullscreenQuad);
}

void DeferredRenderPipeline::DrawDebugInfo(const Scene& scene, const Camera* camera)
{
	RenderState* state = RenderContext::Activate(mDebugEffect);

	state->FindUniform("ProjectionMatrix")->SetMatrix(camera->GetProjectionMatrix());
	state->FindUniform("ViewMatrix")->SetMatrix(camera->GetViewMatrix());
	auto color = state->FindUniform("Color");
	auto objectPosition = state->FindUniform("ObjectPosition");

	LightSourceResultSet::Iterator it = mLightSourceResultSet.GetIterator();
	LightSourceResultSet::Type block;
	while (block = it.Next()) {
		// Ignore spotlights
		if (block->direction.IsNotZero())
			continue;

		color->SetColorRGB(block->color);
		objectPosition->SetVector3(block->position);
		state->Render(mSphere->GetVertexBuffer(), mSphere->GetIndexBuffer());
	}
}
