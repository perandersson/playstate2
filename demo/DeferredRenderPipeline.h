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
	virtual bool OnWindowResized(const Size& newSize);
	virtual void OnSceneGroupAdded(SceneGroup* group);
	virtual void OnSceneGroupRemoved(SceneGroup* group);

	void DrawGeometry(const Scene& scene, const Camera* camera);
	void DrawLighting(const Scene& scene, const Camera* camera);
	bool DrawPointLights(const Scene& scene, const Camera* camera, bool clear);
	bool DrawSpotLights(const Scene& scene, const Camera* camera, bool clear);
	void DrawSpotLightShadows(const Scene& scene, const Camera* camera);

	void DrawFinalResultToScreen(const Scene& scene, const Camera* camera);
	void DrawDebugInfo(const Scene& scene, const Camera* camera);

	/*!
		\brief Retrieves the shadow texture size based on the projector's far clip distance.

		\param farClipDistance
				The far clip distance for a point light projector
		\return
				The size of the shadow
	*/
	Size GetShadowSize(float32 farClipDistance) const;

	/*!
		\brief Retrieves the size of the blur texture, based on the original size

		\param originalSize
		\return
	*/
	Size GetBlurSize(Size originalSize) const;

	/*!
		\brief Retrieves a shared render target based on the supplied render target

		\param rt
		\return
	*/
	RenderTarget2D* GetDepthRenderTarget(RenderTarget2D* rt);

	/*!
		\brief Retrieves a shared render target based on the supplied render target

		\param rt
		\return
	*/
	RenderTarget2D* GetBlurRenderTarget(RenderTarget2D* rt);

	/*!
		\brief Blur the supplied render targets
	
		\param renderTargets
	*/
	void BlurRenderTargets(std::vector<RenderTarget2D*>& renderTargets);

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

	RenderTarget2D* mAlbedoRenderTarget;
	RenderTarget2D* mGeometryRenderTarget;
	RenderTarget2D* mDepthRenderTarget;
	RenderTarget2D* mLightRenderTarget;

	Resource<Effect> mShadowMapEffect;
	ShadowMaps mShadowMaps;
	std::vector<std::shared_ptr<RenderTarget2D>> mDepthRenderTargets;
	std::vector<std::shared_ptr<RenderTarget2D>> mBlurRenderTargets;

	Resource<Effect> mBlurEffect;

	RenderBlockResultSet mRenderBlockResultSet;
	LightSourceResultSet mPointLightsResultSet;
	LightSourceResultSet mSpotLightsResultSet;
};