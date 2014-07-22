#include <core/Memory.h>
#include "DeferredRenderPipeline.h"

DeferredRenderPipeline::DeferredRenderPipeline()
: mFullscreenQuad(nullptr), mSphere(nullptr),
mDiffuseRenderTarget(nullptr), mNormalsRenderTarget(nullptr),
mDepthRenderTarget(nullptr), mLightRenderTarget(nullptr)
{
	ActiveWindow::AddWindowResizedListener(this);

	const Size& windowSize = ActiveWindow::GetSize();

	mDiffuseRenderTarget = RenderContext::CreateRenderTarget2D(windowSize, TextureFormat::RGB16);
	mNormalsRenderTarget = RenderContext::CreateRenderTarget2D(windowSize, TextureFormat::RGBA16F);
	mDepthRenderTarget = RenderContext::CreateRenderTarget2D(windowSize, TextureFormat::DEPTH32F);
	mLightRenderTarget = RenderContext::CreateRenderTarget2D(windowSize, TextureFormat::RGBA16F);

	mGeometryEffect = ResourceManager::GetResource<Effect>("/demo/effects/deferred/geometry.effect");
	mPointLightEffect = ResourceManager::GetResource<Effect>("/demo/effects/deferred/pointlight.effect");
	mSpotLightEffect = ResourceManager::GetResource<Effect>("/demo/effects/deferred/spotlight.effect");
	mResultEffect = ResourceManager::GetResource<Effect>("/demo/effects/deferred/result.effect");
	mDebugEffect = ResourceManager::GetResource<Effect>("/demo/effects/debug.effect");
	mWhiteTexture = ResourceManager::GetResource<Texture2D>("/engine/textures/white.png");
	mBlackTexture = ResourceManager::GetResource<Texture2D>("/engine/textures/black.png");

	mShadowMapEffect = ResourceManager::GetResource<Effect>("/demo/effects/deferred/shadow.effect");
	mBlurEffect = ResourceManager::GetResource<Effect>("/demo/effects/deferred/blur.effect");

	mSphere = Sphere::Create(1, 10, BufferUsage::STATIC);
	mFullscreenQuad = VertexBufferUtils::CreateFullscreenQuad();
	mUniformProjectionMatrix = CameraUtils::GetOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);
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
}

static const std::string PROJECTION_MATRIX("ProjectionMatrix");
static const std::string VIEW_MATRIX("ViewMatrix");
static const std::string MODEL_MATRIX("ModelMatrix");

static const std::string SCREEN_SIZE("ScreenSize");

static const std::string FAR_CLIP_DISTANCE("FarClipDistance");
static const std::string DIFFUSE_TEXTURE("DiffuseTexture");
static const std::string DIFFUSE_COLOR("DiffuseColor");


void DeferredRenderPipeline::Render(const Scene& scene, const Camera* camera)
{
	FindQuery query = { camera->GetFrustum(), FindQueryFilter::DEFAULT };
	// Submit the rendering of the scenes vertex components to another thread.
	// This is not how it should be in the end. Purpose for this is to verify that
	// threaded rendering works as intended
	if (scene.Find(query, &mRenderBlockResultSet)) {
		auto fut = RenderContext::Async<bool>([this, &scene, &camera] {
			RenderState* state = RenderContext::Activate(mGeometryEffect);
			state->SetRenderTarget(mDiffuseRenderTarget, 0);
			state->SetRenderTarget(mNormalsRenderTarget, 1);
			state->SetRenderTarget(mDepthRenderTarget, 2);
			state->Clear(ClearType::COLOR_AND_DEPTH);

			// Set camera properties
			state->FindUniform(PROJECTION_MATRIX)->SetMatrix(camera->GetProjectionMatrix());
			state->FindUniform(VIEW_MATRIX)->SetMatrix(camera->GetViewMatrix());
			state->FindUniform(FAR_CLIP_DISTANCE)->SetFloat(camera->GetFarClipDistance());

			auto modelMatrix = state->FindUniform(MODEL_MATRIX);
			auto diffuseTexture = state->FindUniform(DIFFUSE_TEXTURE);
			auto diffuseColor = state->FindUniform(DIFFUSE_COLOR);

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

void DeferredRenderPipeline::PrepareSceneGroup(Scene* scene, SceneGroup* group)
{
}

bool DeferredRenderPipeline::OnWindowResized(const Size& newSize)
{
	mDiffuseRenderTarget->Resize(newSize);
	mNormalsRenderTarget->Resize(newSize);
	mDepthRenderTarget->Resize(newSize);
	mLightRenderTarget->Resize(newSize);
	return true;
}

void DeferredRenderPipeline::OnSceneGroupAdded(SceneGroup* group)
{
	//
	// Pre-generate static shadows.
	//

	// Collect all spotlights in the scene group
	LightSourceResultSet spotLightResultSet;
	FindQuery query = { nullptr, FindQueryFilter::SPOT_LIGHTS };
	std::vector<ShadowMap*> newShadowMaps;
	if (group->Find(query, &spotLightResultSet)) {
		std::shared_ptr<RenderTarget2D> depthRenderTarget(RenderContext::CreateRenderTarget2D(Size(256, 256), TextureFormat::DEPTH32F));
		RenderState* state = RenderContext::Activate(mShadowMapEffect);

		LightSourceResultSet::Iterator it = spotLightResultSet.GetIterator();
		LightSourceResultSet::Type block;
		while (block = it.Next()) {
			auto lightCamera = block->projector;

			// Create a shadow map container if non exist for the current SpotLight
			ShadowMap* shadowMap;
			auto it = mShadowMaps.find(block->uid);
			if (it == mShadowMaps.end()) {
				shadowMap = new ShadowMap();
				shadowMap->renderTarget = RenderContext::CreateRenderTarget2D(Size(256, 256), TextureFormat::RGBA16F);
				mShadowMaps.insert(std::make_pair(block->uid, shadowMap));
			}
			else
				shadowMap = it->second;

			state->SetRenderTarget(shadowMap->renderTarget, 0);
			state->SetRenderTarget(depthRenderTarget.get(), 1);
			state->Clear(ClearType::COLOR_AND_DEPTH);

			// Collect the non-dynamic shadow casters and generate a shadow map it
			DefaultRenderBlockResultSetSorter sorter;
			RenderBlockResultSet renderBlocks(100, 5);
			FindQuery renderBlocksQuery = { lightCamera->GetFrustum(), FindQueryFilter::STATIC_SHADOW_CASTER | FindQueryFilter::GEOMETRY };
			if (group->Find(renderBlocksQuery, &renderBlocks)) {
				renderBlocks.Sort(&sorter);
				auto projectionMatrix = state->FindUniform(PROJECTION_MATRIX);
				auto viewMatrix = state->FindUniform(VIEW_MATRIX);
				auto modelMatrix = state->FindUniform(MODEL_MATRIX);
				auto farClipDistance = state->FindUniform(FAR_CLIP_DISTANCE);

				projectionMatrix->SetMatrix(lightCamera->GetProjectionMatrix());
				viewMatrix->SetMatrix(lightCamera->GetViewMatrix());
				farClipDistance->SetFloat(lightCamera->GetFarClipDistance());

				RenderBlockResultSet::Iterator renderBlocksIterator = renderBlocks.GetIterator();
				RenderBlockResultSet::Type renderBlock;
				while (renderBlock = renderBlocksIterator.Next()) {
					modelMatrix->SetMatrix(renderBlock->modelMatrix);
					state->Render(renderBlock->vertexBuffer, renderBlock->indexBuffer, renderBlock->startIndex, renderBlock->count);
				}
			}

			newShadowMaps.push_back(shadowMap);
		}
	}

	// Blur the newly generated shadow maps
	if (!newShadowMaps.empty()) {
		RenderState* state = RenderContext::Activate(mBlurEffect);
		state->FindUniform(PROJECTION_MATRIX)->SetMatrix(mUniformProjectionMatrix);
		auto textureToBlur = state->FindUniform("TextureToBlur");
		auto scaleU = state->FindUniform("ScaleU");
		static const float32 shadowCoef = 0.25f;

		size_t size = newShadowMaps.size();
		for (size_t i = 0; i < size; ++i) {
			ShadowMap* shadowMap = newShadowMaps[i];
			const Size size = shadowMap->renderTarget->GetSize();
			std::shared_ptr<RenderTarget2D> temp(RenderContext::CreateRenderTarget2D(Size(size.width * shadowCoef, size.height * shadowCoef), shadowMap->renderTarget->GetTextureFormat()));
			
			// Blur horizontally
			state->SetRenderTarget(temp.get(), 0);
			textureToBlur->SetTexture(shadowMap->renderTarget);
			scaleU->SetVector2(Vector2(1.0f / temp->GetSize().width, 0.0f));
			state->Render(mFullscreenQuad);

			// Blur vertically
			state->SetRenderTarget(shadowMap->renderTarget, 0);
			textureToBlur->SetTexture(temp.get());
			scaleU->SetVector2(Vector2(0.0f, 1.0f / temp->GetSize().height));
			state->Render(mFullscreenQuad);

			// Ensure that the rendering queue is complete
			state->Flush();
		}
	}
}

void DeferredRenderPipeline::OnSceneGroupRemoved(SceneGroup* group)
{
	//
	// Remove cached shadows
	//

	LightSourceResultSet spotLightResultSet;
	FindQuery query = { nullptr, FindQueryFilter::SPOT_LIGHTS };
	if (group->Find(query, &spotLightResultSet)) {
		LightSourceResultSet::Iterator it = spotLightResultSet.GetIterator();
		LightSourceResultSet::Type block;
		while (block = it.Next()) {
			auto it = mShadowMaps.find(block->uid);
			if (it != mShadowMaps.end()) {
				delete it->second->renderTarget;
				delete it->second;
				mShadowMaps.erase(it);
			}
		}
	}
}

void DeferredRenderPipeline::DrawLighting(const Scene& scene, const Camera* camera)
{
	//
	// Setup deferred lighting effect
	//
	bool cleared = DrawSpotLights(scene, camera, true);
	cleared = DrawPointLights(scene, camera, !cleared);
	//DrawSpotLightShadows(scene, camera);


	// TODO: Add Directional lights!
	// TODO: Add Area lights!
}

bool DeferredRenderPipeline::DrawPointLights(const Scene& scene, const Camera* camera, bool clear)
{
	RenderState* state = RenderContext::Activate(mPointLightEffect);
	state->SetRenderTarget(mLightRenderTarget, 0);
	state->SetRenderTarget(mDepthRenderTarget, 1);
	state->SetDepthMask(false);
	if (clear)
		state->Clear(ClearType::COLOR);

	state->FindUniform("NormalsTexture")->SetTexture(mNormalsRenderTarget);

	state->FindUniform(PROJECTION_MATRIX)->SetMatrix(camera->GetProjectionMatrix());
	state->FindUniform(VIEW_MATRIX)->SetMatrix(camera->GetViewMatrix());
	auto modelMatrix = state->FindUniform(MODEL_MATRIX);

	// Screen information
	const Size& size = ActiveWindow::GetSize();
	state->FindUniform(SCREEN_SIZE)->SetVector2(Vector2(size.x, size.y));
	state->FindUniform(FAR_CLIP_DISTANCE)->SetFloat(camera->GetFarClipDistance());

	// Default light texture
	state->FindUniform("LightTexture")->SetTexture(mWhiteTexture);

	FindQuery query = { camera->GetFrustum(), FindQueryFilter::GEOMETRY | FindQueryFilter::POINT_LIGHTS };
	if (scene.Find(query, &mPointLightsResultSet)) {
		auto lightColor = state->FindUniform("LightColor");
		auto lightPosition = state->FindUniform("LightPosition");
		auto constantAttenuation = state->FindUniform("ConstantAttenuation");
		auto linearAttenuation = state->FindUniform("LinearAttenuation");
		auto quadraticAttenuation = state->FindUniform("QuadraticAttenuation");
		auto lightRadius = state->FindUniform("LightRadius");

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
	FindQuery query = { camera->GetFrustum(), FindQueryFilter::GEOMETRY | FindQueryFilter::SPOT_LIGHTS | FindQueryFilter::TEXTURES };
	if (scene.Find(query, &mSpotLightsResultSet)) {
		RenderState* state = RenderContext::Activate(mSpotLightEffect);
		state->SetRenderTarget(mLightRenderTarget, 0);
		state->SetRenderTarget(mDepthRenderTarget, 1);
		state->SetDepthMask(false);
		if (clear)
			state->Clear(ClearType::COLOR);

		state->FindUniform("GeometryTexture")->SetTexture(mNormalsRenderTarget);

		state->FindUniform(PROJECTION_MATRIX)->SetMatrix(camera->GetProjectionMatrix());
		state->FindUniform(VIEW_MATRIX)->SetMatrix(camera->GetViewMatrix());
		auto modelMatrix = state->FindUniform(MODEL_MATRIX);

		// Screen information
		const Size& size = ActiveWindow::GetSize();
		state->FindUniform(SCREEN_SIZE)->SetVector2(Vector2(size.x, size.y));
		state->FindUniform(FAR_CLIP_DISTANCE)->SetFloat(camera->GetFarClipDistance());

		// Default light texture
		auto lightTexture = state->FindUniform("LightTexture");

		auto lightColor = state->FindUniform("LightColor");
		auto lightPosition = state->FindUniform("LightPosition");
		auto constantAttenuation = state->FindUniform("ConstantAttenuation");
		auto linearAttenuation = state->FindUniform("LinearAttenuation");
		auto quadraticAttenuation = state->FindUniform("QuadraticAttenuation");
		auto cosLightCutoff = state->FindUniform("CosLightCutoff");
		auto spotDirection = state->FindUniform("SpotDirection");
		auto spotExponent = state->FindUniform("SpotExponent");
		auto shadowMapTexture = state->FindUniform("ShadowMapTexture");
		shadowMapTexture->SetTextureCompareMode(CompareMode::COMPARE_R_TO_TEXTURE);
		shadowMapTexture->SetTextureCompareFunc(CompareFunc::LEQUAL);
		auto shadowMapMatrix = state->FindUniform("ShadowMapMatrix");
		//static const Matrix4x4 bias(0.5f, 0.0f, 0.0f, 0.5f,
		//							0.0f, 0.5f, 0.0f, 0.5f,
		//							0.0f, 0.0f, 0.5f, 0.5f,
		//							0.0f, 0.0f, 0.0f, 1.0f);
		LightSourceResultSet::Iterator it = mSpotLightsResultSet.GetIterator();
		LightSourceResultSet::Type block;
		while (block = it.Next()) {

			auto shadowMap = mShadowMaps.find(block->uid);
			if (shadowMap != mShadowMaps.end()) {
				shadowMapTexture->SetTexture(shadowMap->second->renderTarget);
			}
			else {
				shadowMapTexture->SetTexture(mWhiteTexture);
			}
			const Matrix4x4 invViewMatrix(camera->GetViewMatrix().GetInverted());
			shadowMapMatrix->SetMatrix(block->projector->GetProjectionMatrix() * block->projector->GetViewMatrix() * invViewMatrix);

			modelMatrix->SetMatrix(Matrix4x4::Translation(block->position));

			lightColor->SetColorRGB(block->color);
			lightPosition->SetVector3(block->position);

			cosLightCutoff->SetFloat(cosf(block->radius * ANG2RAD));
			spotDirection->SetVector3(block->direction);

			if (block->texture == nullptr)
				lightTexture->SetTexture(mWhiteTexture);
			else
				lightTexture->SetTexture(block->texture);

			spotExponent->SetFloat(block->spotExponent);
			state->Render(block->vertexBuffer, block->indexBuffer);
		}
	}

	// TODO: Post shadow render requests to multiple threads here
	return mSpotLightsResultSet.GetSize() > 0;
}

void DeferredRenderPipeline::DrawSpotLightShadows(const Scene& scene, const Camera* camera)
{
	//// Draw shadows from the spotlights point-of-view
	//RenderState* state = RenderContext::Activate(mShadowMapEffect);
	//state->SetRenderTarget(mShadowMapRenderTarget, 0);
	//state->SetRenderTarget(mShadowDepthRenderTarget, 1);
	//state->Clear(ClearType::DEPTH_AND_STENCIL);

	//auto projectionMatrix = state->FindUniform(PROJECTION_MATRIX);
	//auto viewMatrix = state->FindUniform(VIEW_MATRIX);
	//auto modelMatrix = state->FindUniform(MODEL_MATRIX);
	//auto farClipDistance = state->FindUniform(FAR_CLIP_DISTANCE);

	//LightSourceResultSet::Iterator it = mSpotLightsResultSet.GetIterator();
	//LightSourceResultSet::Type block;
	//while (block = it.Next()) {
	//	auto lightCamera = block->projector;

	//	projectionMatrix->SetMatrix(lightCamera->GetProjectionMatrix());
	//	viewMatrix->SetMatrix(lightCamera->GetViewMatrix());
	//	farClipDistance->SetFloat(lightCamera->GetFarClipDistance());

	//	// Locate the geometry found from the lights point of view
	//	static RenderBlockResultSet lightViewBlocks(100, 5);
	//	FindQuery query = { lightCamera->GetFrustum(), FindQueryFilter::GEOMETRY };
	//	if (scene.Find(query, &lightViewBlocks)) {
	//		RenderBlockResultSet::Iterator it2 = lightViewBlocks.GetIterator();
	//		RenderBlockResultSet::Type block2;
	//		while (block2 = it2.Next()) {
	//			modelMatrix->SetMatrix(block2->modelMatrix);
	//			state->Render(block2->vertexBuffer, block2->indexBuffer, block2->startIndex, block2->count);
	//		}
	//	}
	//}
}

void DeferredRenderPipeline::DrawFinalResultToScreen(const Scene& scene, const Camera* camera)
{
	RenderState* state = RenderContext::Activate(mResultEffect);
	state->Clear(ClearType::COLOR);

	state->FindUniform("DiffuseTexture")->SetTexture(mDiffuseRenderTarget);
	state->FindUniform("DepthTexture")->SetTexture(mDepthRenderTarget);
	state->FindUniform("NormalsTexture")->SetTexture(mNormalsRenderTarget);
	state->FindUniform("LightTexture")->SetTexture(mLightRenderTarget);
	state->FindUniform("ShadowMapTexture")->SetTexture(mShadowMaps.begin()->second->renderTarget);
	//state->FindUniform("ShadowDepthMapTexture")->SetTexture(mShadowDepthRenderTarget);

	state->FindUniform("AmbientColor")->SetColorRGB(scene.GetAmbientLight());
	state->FindUniform("ProjectionMatrix")->SetMatrix(mUniformProjectionMatrix);
	state->Render(mFullscreenQuad);
}

void DeferredRenderPipeline::DrawDebugInfo(const Scene& scene, const Camera* camera)
{
	RenderState* state = RenderContext::Activate(mDebugEffect);

	state->FindUniform(PROJECTION_MATRIX)->SetMatrix(camera->GetProjectionMatrix());
	state->FindUniform(VIEW_MATRIX)->SetMatrix(camera->GetViewMatrix());
	auto modelMatrix = state->FindUniform(MODEL_MATRIX);
	auto color = state->FindUniform("Color");

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
