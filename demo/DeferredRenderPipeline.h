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
	bool DrawPointLights(const Scene& scene, const Camera* camera, bool clear);
	bool DrawSpotLights(const Scene& scene, const Camera* camera, bool clear);
	void DrawSpotLightShadows(const Scene& scene, const Camera* camera);

	void DrawFinalResultToScreen(const Scene& scene, const Camera* camera);
	void DrawDebugInfo(const Scene& scene, const Camera* camera);

private:
	Resource<Effect> mGeometryEffect;
	Resource<Effect> mDebugEffect;

	Resource<Effect> mResultEffect;
	Matrix4x4 mUniformProjectionMatrix;
	VertexBuffer* mFullscreenQuad;

	Resource<Effect> mPointLightEffect;
	Sphere* mSphere;

	Resource<Effect> mSpotLightEffect;

	Resource<Texture2D> mWhiteTexture;

	RenderTarget2D* mDiffuseRenderTarget;
	RenderTarget2D* mNormalsRenderTarget;
	RenderTarget2D* mDepthRenderTarget;
	RenderTarget2D* mLightRenderTarget;

	Resource<Effect> mShadowMapEffect;
	RenderTarget2D* mShadowMapRenderTarget;
	RenderTarget2D* mShadowDepthRenderTarget;

	RenderBlockResultSet mRenderBlockResultSet;
	LightSourceResultSet mPointLightsResultSet;
	LightSourceResultSet mSpotLightsResultSet;
};