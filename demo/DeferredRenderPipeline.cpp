#include <core/Memory.h>
#include "DeferredRenderPipeline.h"

DeferredRenderPipeline::DeferredRenderPipeline()
: mFullscreenQuad(nullptr), mUniformSphere(nullptr),
mDiffuseRenderTarget(nullptr), mPositionsRenderTarget(nullptr), mNormalsRenderTarget(nullptr),
mDepthRenderTarget(nullptr), mLightRenderTarget(nullptr), mCubeShadowMap(nullptr)
{
	ActiveWindow::AddWindowResizedListener(this);

	const Size& windowSize = ActiveWindow::GetSize();

	mDiffuseRenderTarget = RenderContext::CreateRenderTarget2D(windowSize, TextureFormat::RGBA);
	mPositionsRenderTarget = RenderContext::CreateRenderTarget2D(windowSize, TextureFormat::RGBA16F);
	mNormalsRenderTarget = RenderContext::CreateRenderTarget2D(windowSize, TextureFormat::RGB10_A2);
	mDepthRenderTarget = RenderContext::CreateRenderTarget2D(windowSize, TextureFormat::DEPTH24);
	mLightRenderTarget = RenderContext::CreateRenderTarget2D(windowSize, TextureFormat::RGBA);

	mCubeShadowMap = RenderContext::CreateRenderTargetCube(Size(256, 256), TextureFormat::DEPTH24);

	mDeferredEffect = ResourceManager::GetResource<Effect>("/demo/effects/deferred/deferred.effect");
	mPointLightEffect = ResourceManager::GetResource<Effect>("/demo/effects/deferred/deferred_pointlight.effect");
	mTexturedEffect = ResourceManager::GetResource<Effect>("/demo/effects/deferred/deferred_result.effect");
	mDebugEffect = ResourceManager::GetResource<Effect>("/demo/effects/debug.effect");
	mWhiteTexture = ResourceManager::GetResource<Texture2D>("/engine/textures/white.png");

	mUniformSphere = VertexBufferUtils::CreateSphere(1, 0, 0, 0, BufferUsage::STATIC);
	mFullscreenQuad = VertexBufferUtils::CreateFullscreenQuad();
}

DeferredRenderPipeline::~DeferredRenderPipeline()
{
	if (mUniformSphere != nullptr) {
		delete mUniformSphere;
		mUniformSphere = nullptr;
	}

	if (mFullscreenQuad != nullptr) {
		delete mFullscreenQuad;
		mFullscreenQuad = nullptr;
	}

	delete mDiffuseRenderTarget;
	delete mPositionsRenderTarget;
	delete mNormalsRenderTarget;
	delete mDepthRenderTarget;
	delete mLightRenderTarget;

	mDiffuseRenderTarget = nullptr;
	mDepthRenderTarget = nullptr;
	mPositionsRenderTarget = nullptr;
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
			state->SetRenderTarget(mPositionsRenderTarget, 1);
			state->SetRenderTarget(mNormalsRenderTarget, 2);
			state->SetDepthRenderTarget(mDepthRenderTarget);
			state->Clear(ClearType::COLOR | ClearType::DEPTH);

			// Set camera properties
			state->FindUniform("ProjectionMatrix")->SetMatrix(camera->GetProjectionMatrix());
			state->FindUniform("ViewMatrix")->SetMatrix(camera->GetViewMatrix());

			IUniform* modelMatrix = state->FindUniform("ModelMatrix");
			IUniform* diffuseTexture = state->FindUniform("DiffuseTexture");
			IUniform* diffuseColor = state->FindUniform("DiffuseColor");

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
	delete mDiffuseRenderTarget;
	delete mPositionsRenderTarget;
	delete mNormalsRenderTarget;
	delete mDepthRenderTarget;
	delete mLightRenderTarget;

	mDiffuseRenderTarget = RenderContext::CreateRenderTarget2D(newSize, TextureFormat::RGBA);
	mPositionsRenderTarget = RenderContext::CreateRenderTarget2D(newSize, TextureFormat::RGBA16F);
	mNormalsRenderTarget = RenderContext::CreateRenderTarget2D(newSize, TextureFormat::RGB10_A2);
	mDepthRenderTarget = RenderContext::CreateRenderTarget2D(newSize, TextureFormat::DEPTH24);
	mLightRenderTarget = RenderContext::CreateRenderTarget2D(newSize, TextureFormat::RGBA);
}

void DeferredRenderPipeline::DrawLighting(const Scene& scene, const Camera* camera)
{
	FindQuery query = { camera, RenderableFilter::DEFAULT };
	RenderState* state = RenderContext::Activate(mPointLightEffect);
	state->SetRenderTarget(mLightRenderTarget, 0);
	state->Clear(ClearType::COLOR);

	if (scene.Find(query, &mLightSourceResultSet)) {
		state->FindUniform("DiffuseTexture")->SetTexture(mDiffuseRenderTarget);
		state->FindUniform("PositionsTexture")->SetTexture(mPositionsRenderTarget);
		state->FindUniform("NormalsTexture")->SetTexture(mNormalsRenderTarget);

		state->FindUniform("ProjectionMatrix")->SetMatrix(camera->GetProjectionMatrix());
		state->FindUniform("ViewMatrix")->SetMatrix(camera->GetViewMatrix());
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
			lightColor->SetColorRGB(block->color);
			lightPosition->SetVector3(block->position);
			constantAttenuation->SetFloat(block->constantAttenuation);
			linearAttenuation->SetFloat(block->linearAttenuation);
			quadraticAttenuation->SetFloat(block->quadricAttenuation);
			lightRadius->SetFloat(block->radius);

			state->Render(mUniformSphere);
		}
	}
}

void DeferredRenderPipeline::DrawFinalResultToScreen(const Scene& scene, const Camera* camera)
{
	RenderState* state = RenderContext::Activate(mTexturedEffect);

	state->FindUniform("DiffuseTexture")->SetTexture(mDiffuseRenderTarget);
	state->FindUniform("PositionsTexture")->SetTexture(mPositionsRenderTarget);
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
		color->SetColorRGB(block->color);
		objectPosition->SetVector3(block->position);
		state->Render(mUniformSphere);
	}
}
