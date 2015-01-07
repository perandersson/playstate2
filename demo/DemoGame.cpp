#include <core/Memory.h>
#include "DemoGame.h"
#include "MovePlayerComponent.h"
#include "FollowCameraComponent.h"
#include <future>
#include <core/StringUtils.h>

DECLARE_GAME(DemoGame);

DemoGame::DemoGame(IKernel* kernel)
: StandardGame(kernel), mNumFrames(0), mTotalTime(0), mRenderPipeline(nullptr), mCamera(nullptr)
{

}

DemoGame::~DemoGame()
{

}

bool DemoGame::Initialize()
{
	if (!StandardGame::Initialize())
		return false;

	//
	// Register custom script types
	//

	auto classLoader = Kernel::GetClassLoader();
	classLoader->GetType("MovePlayerComponent").InheritFrom("Component").AddFunctions(MovePlayerComponent_Methods).Build();
	classLoader->GetType("FollowCameraComponent").InheritFrom("Component").AddFunctions(FollowCameraComponent_Methods).Build();

	//
	// Register how the scene is rendered
	//

	mRenderPipeline = new DeferredRenderPipeline();
	SetRenderPipeline(mRenderPipeline);
	GetScene()->AddSceneGroupListener(mRenderPipeline);


	return true;
}

void DemoGame::Release()
{
	SetRenderPipeline(nullptr);
	delete mRenderPipeline;

	StandardGame::Release();
}

void DemoGame::Update()
{
	mNumFrames++;
	mTotalTime += GameDeltaTime;
	if (mTotalTime > 1.0f) {
		ActiveWindow::SetTitle(std::string("FPS: ") + StringUtils::ToString(mNumFrames));
		mTotalTime -= mTotalTime;
		mNumFrames = 0;
	}
}

void DemoGame::LoadContent(IResourceManager* resourceManager)
{
	auto scene = GetScene();

	auto group = ScriptManager::EvaluateFile("/demo/level1.lua").ToObject<SceneGroup>();
	scene->AddSceneGroup(group);
	auto playerGroup = ScriptManager::EvaluateFile("/demo/player.lua").ToObject<SceneGroup>();
	scene->AddSceneGroup(playerGroup);
	scene->SetAmbientLight(Color(0.1f, 0.1f, 0.1f));

	mCamera = new Camera();
	const Size& windowSize = ActiveWindow::GetSize();
	const float32 ratio = windowSize.width / (float32)windowSize.height;
	mCamera->SetPerspective(1.0f, 200.0f, 45.0f, ratio);
	mCamera->LookAt(Vector3(0, 50.0f, 50.0f), Vector3(0, 0, 0), Vector3(0, 1.0f, 0));

	scene->SetActiveCamera(mCamera);
}

void DemoGame::UnloadContent(IResourceManager* resourceManager)
{
	delete mCamera;
	mCamera = nullptr;
}
