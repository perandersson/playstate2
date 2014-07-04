#include <core/Memory.h>
#include "DeferredRenderPipeline.h"

DeferredRenderPipeline::DeferredRenderPipeline()
: mDiffuseRenderTarget(nullptr), mPositionsRenderTarget(nullptr), mNormalsRenderTarget(nullptr),
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

	mDeferredEffect = ResourceManager::GetResource<Effect>("/demo/effects/deferred.effect");
	mPointLightEffect = ResourceManager::GetResource<Effect>("/demo/effects/deferred_point_light.effect");
	mTexturedEffect = ResourceManager::GetResource<Effect>("/demo/effects/deferred_result.effect");
	//mShadowMappingEffect = ResourceManager::GetResource<Effect>("/demo/effects/shadow_mapping.effect");

	mPointLightTexture = ResourceManager::GetResource<Texture2D>("/demo/effects/light.png");

	PositionTextureNormalVertexType elements[6];
	elements[0].pos = Vector3(-1.0f, 1.0f, 0.0f);
	elements[0].texCoord = Vector2(0.0f, 1.0f);
	elements[1].pos = Vector3(1.0f, 1.0f, 0.0f);
	elements[1].texCoord = Vector2(1.0f, 1.0f);
	elements[2].pos = Vector3(-1.0f, -1.0f, 0.0f);
	elements[2].texCoord = Vector2(0.0f, 0.0f);
	elements[3].pos = Vector3(-1.0f, -1.0f, 0.0f);
	elements[3].texCoord = Vector2(0.0f, 0.0f);
	elements[4].pos = Vector3(1.0f, 1.0f, 0.0f);
	elements[4].texCoord = Vector2(1.0f, 1.0f);
	elements[5].pos = Vector3(1.0f, -1.0f, 0.0f);
	elements[5].texCoord = Vector2(1.0f, 0.0f);
	mFullscreenQuad = std::shared_ptr<VertexBuffer>(RenderContext::CreateBuffer(elements, 6, BufferUsage::STATIC));
}

DeferredRenderPipeline::~DeferredRenderPipeline()
{
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

	//if (scene.Find(query, &mRenderBlockResultSet)) {
	//	RenderState* state = RenderContext::Activate(mDeferredEffect);
	//	state->SetRenderTarget(mDiffuseRenderTarget, 0);
	//	state->SetRenderTarget(mPositionsRenderTarget, 1);
	//	state->SetRenderTarget(mNormalsRenderTarget, 2);
	//	state->SetDepthRenderTarget(mDepthRenderTarget);
	//	state->Clear(ClearType::COLOR | ClearType::DEPTH);

	//	// Set camera properties
	//	state->FindUniform("ProjectionMatrix")->SetMatrix(camera->GetProjectionMatrix());
	//	state->FindUniform("ViewMatrix")->SetMatrix(camera->GetViewMatrix());

	//	IUniform* modelMatrix = state->FindUniform("ModelMatrix");
	//	IUniform* diffuseTexture = state->FindUniform("DiffuseTexture");
	//	IUniform* diffuseColor = state->FindUniform("DiffuseColor");

	//	RenderBlockResultSet::Iterator it = mRenderBlockResultSet.GetIterator();
	//	RenderBlockResultSet::Type block;
	//	while (block = it.Next()) {
	//		diffuseTexture->SetTexture(block->diffuseTexture);
	//		diffuseColor->SetColorRGB(block->diffuseColor);
	//		modelMatrix->SetMatrix(block->modelMatrix);
	//		state->Render(block->vertexBuffer, block->indexBuffer);
	//	}
	//}

	DrawLighting(scene, camera);
	DrawFinalResultToScreen(scene, camera);
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
		state->FindUniform("LightTexture")->SetTexture(mPointLightTexture);

		IUniform* modelMatrix = state->FindUniform("ModelMatrix");
		IUniform* lightColor = state->FindUniform("LightColor");
		IUniform* lightPosition = state->FindUniform("LightPosition");
		IUniform* constantAttenuation = state->FindUniform("ConstantAttenuation");
		IUniform* linearAttenuation = state->FindUniform("LinearAttenuation");
		IUniform* quadraticAttenuation = state->FindUniform("QuadraticAttenuation");
		IUniform* lightRadius = state->FindUniform("LightRadius");

		LightSourceResultSet::Iterator it = mLightSourceResultSet.GetIterator();
		LightSourceResultSet::Type block;
		while (block = it.Next()) {

			// TODO Render point lights as six spot-lights with texture "LightTexture" that's specified above.
			modelMatrix->SetMatrix(CalculateBillboardModelMatrix(block->position, camera));

			lightColor->SetColorRGB(block->color);
			lightPosition->SetVector3(block->position);
			constantAttenuation->SetFloat(block->constantAttenuation);
			linearAttenuation->SetFloat(block->linearAttenuation);
			quadraticAttenuation->SetFloat(block->quadricAttenuation);
			lightRadius->SetFloat(block->radius);

			state->Render(mFullscreenQuad.get());
		}
	}
}

Matrix4x4 DeferredRenderPipeline::CalculateBillboardModelMatrix(const Vector3& position, const Camera* camera)
{
	const Vector3 direction = (camera->GetPosition() - position).GetNormalized();
	const Vector3 right = camera->GetUp().CrossProduct(direction);
	const Vector3 up = direction.CrossProduct(right);

	Matrix4x4 mat(right.x, right.y, right.z, 0.0f,
		up.x, up.y, up.z, 0.0f,
		direction.x, direction.y, direction.z, 0.0f,
		position.x, position.y, position.z, 1.0f);
	return mat;
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
	state->Render(mFullscreenQuad.get());
}
