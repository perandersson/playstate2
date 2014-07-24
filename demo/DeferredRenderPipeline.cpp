#include <core/Memory.h>
#include "DeferredRenderPipeline.h"

DeferredRenderPipeline::DeferredRenderPipeline()
: mFullscreenQuad(nullptr), mSphere(nullptr),
mAlbedoRenderTarget(nullptr), mGeometryRenderTarget(nullptr),
mDepthRenderTarget(nullptr), mLightRenderTarget(nullptr)
{
	ActiveWindow::AddWindowResizedListener(this);

	const Size& windowSize = ActiveWindow::GetSize();

	mAlbedoRenderTarget = RenderContext::CreateRenderTarget2D(windowSize, TextureFormat::RGB16);
	mGeometryRenderTarget = RenderContext::CreateRenderTarget2D(windowSize, TextureFormat::RGBA16F);
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

	// Create depth buffers to use whenever we want to create shadow maps
	mDepthRenderTargets.push_back(std::shared_ptr<RenderTarget2D>(RenderContext::CreateRenderTarget2D(Size(64, 64), TextureFormat::DEPTH32F)));
	mDepthRenderTargets.push_back(std::shared_ptr<RenderTarget2D>(RenderContext::CreateRenderTarget2D(Size(128, 128), TextureFormat::DEPTH32F)));
	mDepthRenderTargets.push_back(std::shared_ptr<RenderTarget2D>(RenderContext::CreateRenderTarget2D(Size(256, 256), TextureFormat::DEPTH32F)));
	mDepthRenderTargets.push_back(std::shared_ptr<RenderTarget2D>(RenderContext::CreateRenderTarget2D(Size(512, 512), TextureFormat::DEPTH32F)));

	// Create blur render targets, used whenever we want to blur a texture
	mBlurRenderTargets.push_back(std::shared_ptr<RenderTarget2D>(RenderContext::CreateRenderTarget2D(GetBlurSize(Size(64, 64)), TextureFormat::RGBA32F)));
	mBlurRenderTargets.push_back(std::shared_ptr<RenderTarget2D>(RenderContext::CreateRenderTarget2D(GetBlurSize(Size(128, 128)), TextureFormat::RGBA32F)));
	mBlurRenderTargets.push_back(std::shared_ptr<RenderTarget2D>(RenderContext::CreateRenderTarget2D(GetBlurSize(Size(256, 256)), TextureFormat::RGBA32F)));
	mBlurRenderTargets.push_back(std::shared_ptr<RenderTarget2D>(RenderContext::CreateRenderTarget2D(GetBlurSize(Size(512, 512)), TextureFormat::RGBA32F)));


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

	delete mAlbedoRenderTarget;
	delete mGeometryRenderTarget;
	delete mDepthRenderTarget;
	delete mLightRenderTarget;

	mAlbedoRenderTarget = nullptr;
	mDepthRenderTarget = nullptr;
	mLightRenderTarget = nullptr;
	mGeometryRenderTarget = nullptr;
}

static const std::string PROJECTION_VIEW_MODEL_MATRIX("ProjectionViewModelMatrix");
static const std::string PROJECTION_VIEW_MATRIX("ProjectionViewMatrix");
static const std::string VIEW_MODEL_MATRIX("ViewModelMatrix");
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
		if (Configuration::ToBool("graphics.multithreading", true)) {
			auto fut = RenderContext::Async<bool>([this, &scene, &camera] {
				DrawGeometry(scene, camera);
				return true;
			});
			fut.get();
		}
		else {
			DrawGeometry(scene, camera);
		}
	}

	DrawLighting(scene, camera);
	DrawFinalResultToScreen(scene, camera);

#if defined(_DEBUG) || defined(RENDERING_TROUBLESHOOTING)
	DrawDebugInfo(scene, camera);
#endif
}

void DeferredRenderPipeline::DrawGeometry(const Scene& scene, const Camera* camera)
{
	RenderState* state = RenderContext::Activate(mGeometryEffect);

	state->SetRenderTarget(mAlbedoRenderTarget, 0);
	state->SetRenderTarget(mGeometryRenderTarget, 1);
	state->SetRenderTarget(mDepthRenderTarget, 2);
	state->Clear(ClearType::COLOR_AND_DEPTH);

	// Set camera properties
	state->FindUniform(PROJECTION_MATRIX)->SetMatrix(camera->GetProjectionMatrix());
	state->FindUniform(FAR_CLIP_DISTANCE)->SetFloat(camera->GetFarClipDistance());

	auto viewModelMatrix = state->FindUniform(VIEW_MODEL_MATRIX);
	auto diffuseTexture = state->FindUniform(DIFFUSE_TEXTURE);
	auto diffuseColor = state->FindUniform(DIFFUSE_COLOR);
	auto normalTexture = state->FindUniform("NormalTexture");

	const Matrix4x4& viewMatrix = camera->GetViewMatrix();
	RenderBlockResultSet::Iterator it = mRenderBlockResultSet.GetIterator();
	RenderBlockResultSet::Type block;
	while (block = it.Next()) {
		if (block->diffuseTexture != nullptr)
			diffuseTexture->SetTexture(block->diffuseTexture);
		else
			diffuseTexture->SetTexture(mWhiteTexture);
		if (block->normalTexture != nullptr)
			normalTexture->SetTexture(block->normalTexture);
		else
			normalTexture->SetTexture(mBlackTexture);
		diffuseColor->SetColorRGB(block->diffuseColor);
		viewModelMatrix->SetMatrix(viewMatrix * block->modelMatrix);
		state->Render(block->vertexBuffer, block->indexBuffer);
	}

	state->End();
}

bool DeferredRenderPipeline::OnWindowResized(const Size& newSize)
{
	mAlbedoRenderTarget->Resize(newSize);
	mGeometryRenderTarget->Resize(newSize);
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
	FindQuery query = { nullptr, FindQueryFilter::SPOT_LIGHTS | FindQueryFilter::STATIC_AND_DYNAMIC_SHADOW_CASTER };
	std::vector<RenderTarget2D*> renderTargetsToBlur;
	if (group->Find(query, &spotLightResultSet)) {
		RenderState* state = RenderContext::Activate(mShadowMapEffect);

		LightSourceResultSet::Iterator it = spotLightResultSet.GetIterator();
		LightSourceResultSet::Type block;
		while (block = it.Next()) {
			auto lightCamera = block->projector;

			// Collect the non-dynamic shadow casters and generate a shadow map with them in it
			DefaultRenderBlockResultSetSorter sorter;
			RenderBlockResultSet renderBlocks(100, 5);
			FindQuery renderBlocksQuery = { lightCamera->GetFrustum(), FindQueryFilter::STATIC_SHADOW_CASTER | FindQueryFilter::GEOMETRY };
			if (group->Find(renderBlocksQuery, &renderBlocks)) {
				renderBlocks.Sort(&sorter);

				// Create a shadow map container if non exist for the current SpotLight
				ShadowMap* shadowMap;
				auto it = mShadowMaps.find(block->uid);
				if (it == mShadowMaps.end()) {
					shadowMap = new ShadowMap();
					shadowMap->renderTarget = RenderContext::CreateRenderTarget2D(GetShadowSize(lightCamera->GetFarClipDistance()), TextureFormat::RGBA32F);
					mShadowMaps.insert(std::make_pair(block->uid, shadowMap));
				}
				else
					shadowMap = it->second;

				// Retrieve and lock a matching depth render target for the shadow map
				RenderTarget2D* depthRenderTarget = GetDepthRenderTarget(shadowMap->renderTarget);
				state->SetRenderTarget(shadowMap->renderTarget, 0);
				state->SetRenderTarget(depthRenderTarget, 1);
				state->Clear(ClearType::COLOR_AND_DEPTH);

				auto projectionViewModelMatrix = state->FindUniform(PROJECTION_VIEW_MODEL_MATRIX);
				
				// Supply the far clip distance so that we can create a linearized depth value
				state->FindUniform(FAR_CLIP_DISTANCE)->SetFloat(lightCamera->GetFarClipDistance());

				RenderBlockResultSet::Iterator renderBlocksIterator = renderBlocks.GetIterator();
				RenderBlockResultSet::Type renderBlock;
				while (renderBlock = renderBlocksIterator.Next()) {
					projectionViewModelMatrix->SetMatrix(lightCamera->GetProjectionViewMatrix() * renderBlock->modelMatrix);
					state->Render(renderBlock->vertexBuffer, renderBlock->indexBuffer, renderBlock->startIndex, renderBlock->count);
				}

				// Flush the rendering queue and then return the depth render target
				state->Flush();

				renderTargetsToBlur.push_back(shadowMap->renderTarget);
			}
		}
		state->End();
	}

	BlurRenderTargets(renderTargetsToBlur);
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

	state->FindUniform("GeometryTexture")->SetTexture(mGeometryRenderTarget);

	state->FindUniform(PROJECTION_MATRIX)->SetMatrix(camera->GetProjectionMatrix());
	state->FindUniform(VIEW_MATRIX)->SetMatrix(camera->GetViewMatrix());
	auto modelMatrix = state->FindUniform(MODEL_MATRIX);

	// Screen information
	const Size& size = ActiveWindow::GetSize();
	state->FindUniform(SCREEN_SIZE)->SetVector2(Vector2(size.x, size.y));
	state->FindUniform(FAR_CLIP_DISTANCE)->SetFloat(camera->GetFarClipDistance());

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
	
	state->End();

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

		state->FindUniform("GeometryTexture")->SetTexture(mGeometryRenderTarget);

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
		//shadowMapTexture->SetTextureCompareMode(CompareMode::COMPARE_R_TO_TEXTURE);
		//shadowMapTexture->SetTextureCompareFunc(CompareFunc::LEQUAL);
		auto shadowMapMatrix = state->FindUniform("ShadowMapMatrix");
		auto lightFarClipDistance = state->FindUniform("LightFarClipDistance");
		static const Matrix4x4 bias(0.5f, 0.0f, 0.0f, 0.5f,
									0.0f, 0.5f, 0.0f, 0.5f,
									0.0f, 0.0f, 0.5f, 0.5f,
									0.0f, 0.0f, 0.0f, 1.0f);
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
			shadowMapMatrix->SetMatrix(bias * block->projector->GetProjectionMatrix() * block->projector->GetViewMatrix() * invViewMatrix);
			lightFarClipDistance->SetFloat(block->projector->GetFarClipDistance());
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

		state->End();
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

	state->FindUniform("AlbedoTexture")->SetTexture(mAlbedoRenderTarget);
	state->FindUniform("LightTexture")->SetTexture(mLightRenderTarget);
	state->FindUniform("GeometryTexture")->SetTexture(mGeometryRenderTarget);

	state->FindUniform("AmbientColor")->SetColorRGB(scene.GetAmbientLight());
	state->FindUniform("ProjectionMatrix")->SetMatrix(mUniformProjectionMatrix);
	state->Render(mFullscreenQuad);

	state->End();
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

	state->End();
}

Size DeferredRenderPipeline::GetShadowSize(float32 farClipDistance) const
{
	// TODO: Implement support fur customized shadow texture sizes
	return Size(512, 512);
}

Size DeferredRenderPipeline::GetBlurSize(Size originalSize) const
{
	static const float32 coef = 0.25f;
	return originalSize * coef;
}

RenderTarget2D* DeferredRenderPipeline::GetDepthRenderTarget(RenderTarget2D* rt)
{
	assert_not_null(rt);

	Size rtSize = rt->GetSize();
	const size_t size = mDepthRenderTargets.size();
	for (size_t i = 0; i < size; ++i) {
		auto sharedRT = mDepthRenderTargets[i].get();
		if (sharedRT->GetSize() == rtSize) {
			return sharedRT;
		}
	}

	THROW_EXCEPTION(RenderingException, "Could not find depth render target for the size: %d %d", rtSize.width, rtSize.height);
}

RenderTarget2D* DeferredRenderPipeline::GetBlurRenderTarget(RenderTarget2D* rt)
{
	assert_not_null(rt);

	Size rtSize = GetBlurSize(rt->GetSize());
	const size_t size = mBlurRenderTargets.size();
	for (size_t i = 0; i < size; ++i) {
		auto sharedRT = mBlurRenderTargets[i].get();
		if (sharedRT->GetSize() == rtSize) {
			return sharedRT;
		}
	}

	THROW_EXCEPTION(RenderingException, "Could not find depth render target for the size: %d %d", rtSize.width, rtSize.height);
}

void DeferredRenderPipeline::BlurRenderTargets(std::vector<RenderTarget2D*>& renderTargets)
{
	if (renderTargets.empty())
		return;

	RenderState* state = RenderContext::Activate(mBlurEffect);
	state->FindUniform(PROJECTION_MATRIX)->SetMatrix(mUniformProjectionMatrix);
	auto textureToBlur = state->FindUniform("TextureToBlur");
	auto scaleU = state->FindUniform("ScaleU");

	const size_t size = renderTargets.size();
	for (size_t i = 0; i < size; ++i) {
		RenderTarget2D* rt = renderTargets[i];
		const Size size = rt->GetSize();
		auto blurRT = GetBlurRenderTarget(rt);

		// Blur horizontally
		state->SetRenderTarget(blurRT, 0);
		textureToBlur->SetTexture(rt);
		scaleU->SetVector2(Vector2(1.0f / blurRT->GetSize().width, 0.0f));
		state->Render(mFullscreenQuad);

		// Blur vertically
		state->SetRenderTarget(rt, 0);
		textureToBlur->SetTexture(blurRT);
		scaleU->SetVector2(Vector2(0.0f, 1.0f / blurRT->GetSize().height));
		state->Render(mFullscreenQuad);

		// Ensure that the rendering queue is complete and return the blur render target
		state->Flush();
	}

	state->End();
}
