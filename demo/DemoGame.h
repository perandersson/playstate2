#pragma once
#include <core/playstate.h>
#include "DeferredRenderPipeline.h"
using namespace core;

class DemoGame : public StandardGame
{
public:
	DemoGame(IKernel* kernel);
	virtual ~DemoGame();

	virtual bool Initialize();
	virtual void Release();
	virtual void Update();
	virtual void LoadContent(IResourceManager* resourceManager);
	virtual void UnloadContent(IResourceManager* resourceManager);

	void DrawLighting(const Scene& scene, const Camera* camera);
	Matrix4x4 CalculateBillboardModelMatrix(const Vector3& position, const Camera* camera);
	void DrawFinalResultToScreen(const Scene& scene, const Camera* camera);

private:
	uint32 mNumFrames;
	float32 mTotalTime;
	DeferredRenderPipeline* mRenderPipeline;
	Camera* mCamera;
};

