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
	void DrawFinalResultToScreen(const Scene& scene, const Camera* camera);
	void DrawDebugInfo(const Scene& scene, const Camera* camera);

private:
	Resource<Effect> mDeferredEffect;
	Resource<Effect> mTexturedEffect;
	Resource<Effect> mDebugEffect;
	VertexBuffer* mFullscreenQuad;

	Resource<Effect> mPointLightEffect;
	Sphere* mSphere;
	Resource<Texture2D> mWhiteTexture;


	RenderTarget2D* mDiffuseRenderTarget;
	//RenderTarget2D* mMaterialsRenderTarget;
	RenderTarget2D* mNormalsRenderTarget;
	RenderTarget2D* mDepthRenderTarget;
	RenderTarget2D* mLightRenderTarget;

	RenderTargetCube* mCubeShadowMap;

	RenderBlockResultSet mRenderBlockResultSet;
	LightSourceResultSet mPointLightsResultSet;
	LightSourceResultSet mSpotLightsResultSet;
};