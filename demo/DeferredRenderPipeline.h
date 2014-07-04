#pragma once
#include <core/playstate.h>
using namespace core;

class DeferredRenderPipeline : public IRenderPipeline, public IWindowResizedListener
{
public:
	DeferredRenderPipeline();
	virtual ~DeferredRenderPipeline();
	
	virtual void Render(const Scene& scene, const Camera* camera);
	virtual void OnWindowResized(const Size& newSize);

	void DrawLighting(const Scene& scene, const Camera* camera);
	Matrix4x4 CalculateBillboardModelMatrix(const Vector3& position, const Camera* camera);
	void DrawFinalResultToScreen(const Scene& scene, const Camera* camera);

private:
	Resource<Effect> mDeferredEffect;
	Resource<Effect> mTexturedEffect;
	Resource<Effect> mShadowMappingEffect;
	Resource<Effect> mPointLightEffect;
	std::shared_ptr<VertexBuffer> mFullscreenQuad;
	Resource<Texture2D> mPointLightTexture;
	Resource<Texture2D> mWhiteTexture;

	RenderTarget2D* mDiffuseRenderTarget;
	RenderTarget2D* mPositionsRenderTarget;
	RenderTarget2D* mNormalsRenderTarget;
	RenderTarget2D* mDepthRenderTarget;
	RenderTarget2D* mLightRenderTarget;

	RenderTargetCube* mCubeShadowMap;

	RenderBlockResultSet mRenderBlockResultSet;
	LightSourceResultSet mLightSourceResultSet;
};