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

	mGeometryEffect = ResourceManager::GetResource<Effect>("/demo/effects/deferred/geometry.effect");
	mPointLightEffect = ResourceManager::GetResource<Effect>("/demo/effects/deferred/pointlight.effect");
	mSpotLightEffect = ResourceManager::GetResource<Effect>("/demo/effects/deferred/spotlight.effect");
	mResultEffect = ResourceManager::GetResource<Effect>("/demo/effects/deferred/result.effect");
	mDebugEffect = ResourceManager::GetResource<Effect>("/demo/effects/debug.effect");
	mWhiteTexture = ResourceManager::GetResource<Texture2D>("/engine/textures/white.png");

	mSphere = Sphere::Create(1, 10, BufferUsage::STATIC);
	mFullscreenQuad = VertexBufferUtils::CreateFullscreenQuad();
}

DeferredRenderPipeline::~DeferredRenderPipeline()
{
	if (mSphere != nullptr) {
		delete mSphere;
		mSphere = nullptr;
	}

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
	FindQuery query = { camera, FindQueryFilter::DEFAULT };
	// Submit the rendering of the scenes vertex components to another thread.
	// This is not how it should be in the end. Purpose for this is to verify that
	// threaded rendering works as intended
	if (scene.Find(query, &mRenderBlockResultSet)) {
		auto fut = RenderContext::Async<bool>([this, &scene, &camera] {
			RenderState* state = RenderContext::Activate(mGeometryEffect);
			state->SetRenderTarget(mDiffuseRenderTarget, 0);
			state->SetRenderTarget(mNormalsRenderTarget, 1);
			state->SetDepthRenderTarget(mDepthRenderTarget);
			state->Clear(ClearType::COLOR | ClearType::DEPTH);

			// Set camera properties
			state->FindUniform("ProjectionMatrix")->SetMatrix(camera->GetProjectionMatrix());
			state->FindUniform("ViewMatrix")->SetMatrix(camera->GetViewMatrix());
			state->FindUniform("FarClipDistance")->SetFloat(camera->GetFarClipDistance());

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
	//
	// Setup deferred lighting effect
	//
	bool cleared = DrawSpotLights(scene, camera, true);
	cleared = DrawPointLights(scene, camera, !cleared);
	// TODO: Add Directional lights!
	// TODO: Add Area lights!
}

bool DeferredRenderPipeline::DrawPointLights(const Scene& scene, const Camera* camera, bool clear)
{
	RenderState* state = RenderContext::Activate(mPointLightEffect);
	state->SetRenderTarget(mLightRenderTarget, 0);
	state->SetDepthRenderTarget(mDepthRenderTarget);
	state->SetDepthMask(false);
	if (clear)
		state->Clear(ClearType::COLOR);

	state->FindUniform("NormalsTexture")->SetTexture(mNormalsRenderTarget);

	state->FindUniform("ProjectionMatrix")->SetMatrix(camera->GetProjectionMatrix());
	state->FindUniform("ViewMatrix")->SetMatrix(camera->GetViewMatrix());
	auto modelMatrix = state->FindUniform("ModelMatrix");

	// Screen information
	const Size& size = ActiveWindow::GetSize();
	state->FindUniform("ScreenSize")->SetVector2(Vector2(size.x, size.y));
	state->FindUniform("FarClipDistance")->SetFloat(camera->GetFarClipDistance());

	// Default light texture
	state->FindUniform("LightTexture")->SetTexture(mWhiteTexture);

	FindQuery query = { camera, FindQueryFilter::DEFAULT | FindQueryFilter::POINT_LIGHTS };
	if (scene.Find(query, &mPointLightsResultSet)) {
		IUniform* lightColor = state->FindUniform("LightColor");
		IUniform* lightPosition = state->FindUniform("LightPosition");
		IUniform* constantAttenuation = state->FindUniform("ConstantAttenuation");
		IUniform* linearAttenuation = state->FindUniform("LinearAttenuation");
		IUniform* quadraticAttenuation = state->FindUniform("QuadraticAttenuation");
		IUniform* lightRadius = state->FindUniform("LightRadius");

		LightSourceResultSet::Iterator it = mPointLightsResultSet.GetIterator();
		LightSourceResultSet::Type block;
		while (block = it.Next()) {
			Matrix4x4 m = Matrix4x4::Translation(block->position) * Matrix4x4::Scale(block->radius, block->radius, block->radius);
			modelMatrix->SetMatrix(m);

			lightColor->SetColorRGB(block->color);
			lightPosition->SetVector3(block->position);
			constantAttenuation->SetFloat(block->constantAttenuation);
			linearAttenuation->SetFloat(block->linearAttenuation);
			quadraticAttenuation->SetFloat(block->quadricAttenuation);
			lightRadius->SetFloat(block->radius);

			state->Render(mSphere->GetVertexBuffer(), mSphere->GetIndexBuffer());
		}
	}

	return mPointLightsResultSet.GetSize() > 0;
}

bool DeferredRenderPipeline::DrawSpotLights(const Scene& scene, const Camera* camera, bool clear)
{
	// Draw spotlights
	FindQuery query = { camera, FindQueryFilter::DEFAULT | FindQueryFilter::SPOT_LIGHTS };
	if (scene.Find(query, &mSpotLightsResultSet)) {
		RenderState* state = RenderContext::Activate(mSpotLightEffect);
		state->SetRenderTarget(mLightRenderTarget, 0);
		state->SetDepthRenderTarget(mDepthRenderTarget);
		state->SetDepthMask(false);
		if (clear)
			state->Clear(ClearType::COLOR);

		state->FindUniform("NormalsTexture")->SetTexture(mNormalsRenderTarget);

		state->FindUniform("ProjectionMatrix")->SetMatrix(camera->GetProjectionMatrix());
		state->FindUniform("ViewMatrix")->SetMatrix(camera->GetViewMatrix());
		auto modelMatrix = state->FindUniform("ModelMatrix");

		// Screen information
		const Size& size = ActiveWindow::GetSize();
		state->FindUniform("ScreenSize")->SetVector2(Vector2(size.x, size.y));
		state->FindUniform("FarClipDistance")->SetFloat(camera->GetFarClipDistance());

		// Default light texture
		state->FindUniform("LightTexture")->SetTexture(mWhiteTexture);

		IUniform* lightColor = state->FindUniform("LightColor");
		IUniform* lightPosition = state->FindUniform("LightPosition");
		IUniform* constantAttenuation = state->FindUniform("ConstantAttenuation");
		IUniform* linearAttenuation = state->FindUniform("LinearAttenuation");
		IUniform* quadraticAttenuation = state->FindUniform("QuadraticAttenuation");
		IUniform* lightCutoff = state->FindUniform("LightCutoff");
		IUniform* cosLightCutoff = state->FindUniform("CosLightCutoff");
		IUniform* spotDirection = state->FindUniform("SpotDirection");

		LightSourceResultSet::Iterator it = mSpotLightsResultSet.GetIterator();
		LightSourceResultSet::Type block;
		while (block = it.Next()) {
			modelMatrix->SetMatrix(Matrix4x4::Translation(block->position));

			lightColor->SetColorRGB(block->color);
			lightPosition->SetVector3(block->position);

			lightCutoff->SetFloat(block->radius);
			cosLightCutoff->SetFloat(cosf(block->radius * ANG2RAD));
			spotDirection->SetVector3(block->direction);

			state->Render(block->vertexBuffer, block->indexBuffer);
		}
	}

	// TODO: Post shadow render requests to multiple threads here
	return mSpotLightsResultSet.GetSize() > 0;
}

void DeferredRenderPipeline::DrawFinalResultToScreen(const Scene& scene, const Camera* camera)
{
	RenderState* state = RenderContext::Activate(mResultEffect);

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
	//state->SetDepthRenderTarget(mDepthRenderTarget);
	//state->SetDepthMask(false);

	state->FindUniform("ProjectionMatrix")->SetMatrix(camera->GetProjectionMatrix());
	state->FindUniform("ViewMatrix")->SetMatrix(camera->GetViewMatrix());
	auto modelMatrix = state->FindUniform("ModelMatrix");
	auto color = state->FindUniform("Color");
	//auto objectPosition = state->FindUniform("ObjectPosition");

	LightSourceResultSet::Iterator it = mPointLightsResultSet.GetIterator();
	LightSourceResultSet::Type block;
	while (block = it.Next()) {
		color->SetColorRGBA(block->color);

		modelMatrix->SetMatrix(Matrix4x4::Translation(block->position));
		state->SetPolygonMode(PolygonMode::FILL);
		state->Render(mSphere->GetVertexBuffer(), mSphere->GetIndexBuffer());
	}

	it = mSpotLightsResultSet.GetIterator();
	while (block = it.Next()) {
		color->SetColorRGBA(block->color);

		modelMatrix->SetMatrix(Matrix4x4::Translation(block->position) * Matrix4x4::Scale(0.1f, 0.1f, 0.1f));
		state->SetPolygonMode(PolygonMode::FILL);
		state->Render(block->vertexBuffer, block->indexBuffer);

		modelMatrix->SetMatrix(Matrix4x4::Translation(block->position));
		state->SetPolygonMode(PolygonMode::LINE);
		state->Render(block->vertexBuffer, block->indexBuffer);

	}



}
