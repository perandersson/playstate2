#include "../Memory.h"
#include "StandardGame.h"
#include "../kernel/Kernel.h"
#include "../rendering/TextureResourceLoader.h"
#include "../rendering/effect/EffectResourceLoader.h"
#include "../rendering/effect/ShaderObjectResourceLoader.h"
#include "../model/PSMFResourceLoader.h"
#include "../model/WaveFrontResourceLoader.h"
#include "../scene/Updatable.h"
#include "../scene/Tickable.h"
#include "../script/ScriptManager.h"
#include "../script/IScriptEnum.h"
#include "../rendering/FrontFace.h"
#include "../rendering/CullFace.h"
#include "../rendering/DepthFunc.h"
#include "../rendering/BlendFunc.h"
#include "../rendering/MagFilter.h"
#include "../rendering/MinFilter.h"
#include "../rendering/TextureWrap.h"
#include "../configuration/Configuration.h"
#include "../filesystem/FileSystem.h"
#include "../logging/Logger.h"
#include "../font/TrueTypeFontResourceLoader.h"

//
// Script integration
//

#include "../integration/ActiveWindow_script.h"
#include "../integration/Game_script.h"
#include "../integration/Music_script.h"
#include "../integration/SoundEffect_script.h"
#include "../integration/SoundEngine_script.h"
#include "../integration/SoundReceiver_script.h"
#include "../integration/Camera_script.h"
#include "../integration/Component_script.h"
#include "../integration/Scene_script.h"
#include "../integration/SceneGroup_script.h"
#include "../integration/LinkedListSceneGroup_script.h"
#include "../integration/QuadTreeSceneGroup_script.h"
#include "../integration/SceneNode_script.h"
#include "../integration/InputDevices_script.h"
#include "../integration/MouseDevice_script.h"
#include "../integration/KeyboardDevice_script.h"
#include "../integration/Renderable_script.h"
#include "../integration/RenderModelSceneNode_script.h"
#include "../integration/LightSource_script.h"
#include "../integration/PointLight_script.h"
#include "../integration/SpotLight_script.h"
#include "../integration/Resource_script.h"
using namespace core;


StandardGame::StandardGame(IKernel* kernel)
: IGame(), mKernel(kernel), mRunning(true), mRenderPipeline(nullptr)
{
	assert_not_null(kernel);
}

StandardGame::~StandardGame()
{
}

bool StandardGame::Process(float64 dt)
{
	// Make sure we are allowed to access all features of the resource manager
	IResourceManager* resourceManager = mKernel->GetResourceManager();
	resourceManager->SetAccessibility(ResourceAccess::ALL);

	// Update delta time
	GameDeltaTime = (float32)dt;
	GameTotalTime += dt;
	
	// Set the global variables associated to this frame
	IScriptManager* scriptManager = mKernel->GetScriptManager();
	scriptManager->SetGlobalVar("GameDeltaTime", GameDeltaTime);
	scriptManager->SetGlobalVar("GameTotalTime", GameTotalTime);

	// Update the scene
	mScene.Update();

	// Update other game logic
	Update();

	// You are only allowed to perform "load" operations on the resource manager during the render phase
	resourceManager->SetAccessibility(ResourceAccess::LOADING);
	Render();
	return mRunning;
}

void StandardGame::Start()
{
	IResourceManager* resourceManager = mKernel->GetResourceManager();
	resourceManager->SetAccessibility(ResourceAccess::ALL);
	LoadContent(resourceManager);

	// The game is now running
	mRunning = true;
}

void StandardGame::Stop()
{
	// Stop the game from running
	mRunning = false;
}

bool StandardGame::Initialize()
{
	//
	// Register default resource types
	//

	IResourceManager* resourceManager = mKernel->GetResourceManager();
	resourceManager->RegisterLoader(new TextureResourceLoader(), "png");
	resourceManager->RegisterLoader(new TextureResourceLoader(), "jpg");
	resourceManager->RegisterLoader(new TextureResourceLoader(), "bmp");
	resourceManager->RegisterLoader(new TextureResourceLoader(), "tiff");
	resourceManager->RegisterLoader(new PSMFResourceLoader(), "psmf");
	resourceManager->RegisterLoader(new WavefrontResourceLoader(), "obj");
	resourceManager->RegisterLoader(new TrueTypeFontResourceLoader(), "ttf");
	resourceManager->RegisterLoader(new EffectResourceLoader(), "effect");
	resourceManager->RegisterLoader(new ShaderObjectResourceLoader(GL_VERTEX_SHADER), "vs");
	resourceManager->RegisterLoader(new ShaderObjectResourceLoader(GL_FRAGMENT_SHADER), "fs");
	resourceManager->RegisterLoader(new ShaderObjectResourceLoader(GL_GEOMETRY_SHADER), "gs");

	IClassLoader* classLoader = Kernel::GetClassLoader();

	//
	// Register script types
	//

	classLoader->GetType("IRenderPipeline").Build();
	classLoader->GetType("ActiveWindow").AddFunctions(ActiveWindow_Methods).Build();
	classLoader->GetType("Game").AddFunctions(Game_Methods).Build();
	classLoader->GetType("Music").AddFunctions(Music_Methods).Build();
	classLoader->GetType("Camera").AddFunctions(Camera_Methods).Build();
	classLoader->GetType("SoundEffect").AddFunctions(SoundEffect_Methods).Build();
	classLoader->GetType("SoundEngine").AddFunctions(SoundEngine_Methods).Build();
	classLoader->GetType("SoundReceiver").AddFunctions(SoundReceiver_Methods).Build();
	classLoader->GetType("Component").AddFunctions(Component_Methods).Build();
	
	classLoader->GetType("Scene").AddFunctions(Scene_Methods).Build();
	
	classLoader->GetType("SceneGroup").AddFunctions(SceneGroup_Methods).Build();
	classLoader->GetType("LinkedListSceneGroup").InheritFrom("SceneGroup").AddFunctions(LinkedListSceneGroup_Methods).Build();
	classLoader->GetType("QuadTreeSceneGroup").InheritFrom("SceneGroup").AddFunctions(QuadTreeSceneGroup_Methods).Build();
	
	classLoader->GetType("SceneNode").AddFunctions(SceneNode_Methods).Build(); 
	classLoader->GetType("SpatialSceneNode").InheritFrom("SceneNode").Build();
	classLoader->GetType("Renderable").InheritFrom("SpatialSceneNode").AddFunctions(Renderable_Methods).Build();
	classLoader->GetType("RenderModelSceneNode").InheritFrom("Renderable").AddFunctions(RenderModelSceneNode_Methods).Build();
	classLoader->GetType("LightSource").InheritFrom("SpatialSceneNode").AddFunctions(LightSource_Methods).Build();
	classLoader->GetType("PointLight").InheritFrom("LightSource").AddFunctions(PointLight_Methods).Build();
	classLoader->GetType("SpotLight").InheritFrom("LightSource").AddFunctions(SpotLight_Methods).Build();

	classLoader->GetType("InputDevices").AddFunctions(InputDevices_Methods).Build();
	classLoader->GetType("KeyboardDevice").AddFunctions(KeyboardDevice_Methods).Build();
	classLoader->GetType("MouseDevice").AddFunctions(MouseDevice_Methods).Build();
	
	classLoader->GetType("Resource").AddFunctions(Resource_Methods).Build();

	//
	// Register enums
	//

	classLoader->GetEnum("BufferUsage").Enums(BufferUsage::GetValues()).Build();
	classLoader->GetEnum("CompareFunc").Enums(CompareFunc::GetValues()).Build();
	classLoader->GetEnum("CompareMode").Enums(CompareMode::GetValues()).Build();
	classLoader->GetEnum("FrontFace").Enums(FrontFace::GetValues()).Build();
	classLoader->GetEnum("CullFace").Enums(CullFace::GetValues()).Build();
	classLoader->GetEnum("DepthFunc").Enums(DepthFunc::GetValues()).Build();
	classLoader->GetEnum("DestFactor").Enums(DestFactor::GetValues()).Build();
	classLoader->GetEnum("SrcFactor").Enums(SrcFactor::GetValues()).Build();
	classLoader->GetEnum("MagFilter").Enums(MagFilter::GetValues()).Build();
	classLoader->GetEnum("MinFilter").Enums(MinFilter::GetValues()).Build();
	classLoader->GetEnum("PolygonMode").Enums(PolygonMode::GetValues()).Build();
	classLoader->GetEnum("TextureWrap").Enums(TextureWrap::GetValues()).Build();

	classLoader->GetEnum("MouseButton").Enums(MouseButton::GetValues()).Build();
	classLoader->GetEnum("KeyboardKey").Enums(KeyboardKey::GetValues()).Build();

	// Listen for all changes made in .lua files if in development mode
	if (Configuration::ToBool("script.developmentmode", false)) {
		FileSystem::AddFileChangedListener(std::regex("[^\\s]+\\.lua$"), this);
	}

	return true;
}

void StandardGame::Release()
{
	// Unload resources
	IResourceManager* resourceManager = mKernel->GetResourceManager();
	resourceManager->SetAccessibility(ResourceAccess::ALL);
	UnloadContent(resourceManager);

	// Stop listening for changes made in .lua files if in development mode
	if (Configuration::ToBool("script.developmentmode", false)) {
		FileSystem::RemoveFileChangedListener(this);
	}
}

void StandardGame::Update()
{
}

void StandardGame::Render()
{
	// Render the scene with the attached render pipeline
	mRenderPipeline->Render(mScene, mScene.GetActiveCamera());
}

void StandardGame::LoadContent(IResourceManager* resourceManager)
{

}

void StandardGame::UnloadContent(IResourceManager* resourceManager)
{

}

Scene* StandardGame::GetScene()
{
	return &mScene;
}

void StandardGame::SetRenderPipeline(IRenderPipeline* pipeline)
{
	mRenderPipeline = pipeline;
}

void StandardGame::OnFileChanged(const IFile* file, FileChangeAction::Enum action)
{
	try {
		std::shared_ptr<IScriptFile> scriptFile = ScriptManager::CompileFile(file->GetAbsolutePath());
		if (action == FileChangeAction::MODIFIED) {
			Logger::Debug("Updated script file: '%s'", file->GetAbsolutePath().c_str());
		}
		else if (action == FileChangeAction::ADDED) {
			Logger::Debug("Added script file: '%s'", file->GetAbsolutePath().c_str());
		}
		else if (action == FileChangeAction::DELETED) {
			Logger::Debug("Deleted script file: '%s'", file->GetAbsolutePath().c_str());
		}
	}
	catch (ScriptException e) {
		Logger::Error("Could not update script file: '%s'. Reason: '%s'", file->GetAbsolutePath().c_str(), e.GetMessage().c_str());
	}
}
