#pragma once
#include <core/playstate.h>
using namespace core;

struct ShadowMap
{
	RenderTarget2D* renderTarget;
};

class DeferredRenderPipeline : public IRenderPipeline, public SceneGroupListener, public WindowResizedListener
{
	typedef std::hash_map<uint32, ShadowMap*> ShadowMaps;

public:
	DeferredRenderPipeline();
	virtual ~DeferredRenderPipeline();
	
	virtual void Render(const Scene& scene, const Camera* camera);
	virtual void PrepareSceneGroup(Scene* scene, SceneGroup* group);
	virtual bool OnWindowResized(const Size& newSize);
	virtual void OnSceneGroupAdded(SceneGroup* group);
	virtual void OnSceneGroupRemoved(SceneGroup* group);

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
	Resource<Texture2D> mBlackTexture;

	RenderTarget2D* mDiffuseRenderTarget;
	RenderTarget2D* mNormalsRenderTarget;
	RenderTarget2D* mDepthRenderTarget;
	RenderTarget2D* mLightRenderTarget;

	Resource<Effect> mShadowMapEffect;
	ShadowMaps mShadowMaps;

	Resource<Effect> mBlurEffect;

	RenderBlockResultSet mRenderBlockResultSet;
	LightSourceResultSet mPointLightsResultSet;
	LightSourceResultSet mSpotLightsResultSet;
};