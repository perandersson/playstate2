#include "../../Memory.h"
#include "Win32Kernel.h"
#include "../Kernel.h"
#include "../../sound/openal/OpenALSoundEffectResourceLoader.h"
#include "../../sound/openal/OpenALMusicResourceLoader.h"
using namespace core;

Win32Kernel::Win32Kernel()
: mConfiguration(nullptr), mFileSystem(nullptr), mInputDevices(nullptr), mGame(nullptr),
mLogger(nullptr), mResourceManager(nullptr), mScriptManager(nullptr), mSoundEngine(nullptr),
mActiveWindow(nullptr), mThreadPool(nullptr)
{
}

Win32Kernel::~Win32Kernel()
{
}

bool Win32Kernel::Initialize()
{
	Kernel::SetInstance(this);

	mLogger = new StdCoutLog();
	mFileSystem = new Win32FileSystem(std::string("data"));
	mScriptManager = new LuaScriptManager(mFileSystem);
	mConfiguration = new LuaConfiguration(mScriptManager, std::string("/config.lua"));
	mSoundEngine = new OpenALSoundEngine(mConfiguration);
	mInputDevices = new Win32InputDevices();
	mActiveWindow = new Win32ActiveWindow(GetModuleHandle(NULL), mConfiguration, mInputDevices);
	try {
		mActiveWindow->Initialize();
	}
	catch (Exception e) {
		mActiveWindow->Alert(std::string("Could not initialize window"), e, AlertType::ALERT_ERROR);
		return false;
	}
	mResourceManager = new ThreadSafeResourceManager(mFileSystem);
	mThreadPool = new ThreadPool();
	mThreadPool->Initialize(4);

	// Initialize a free render contexts that will, later on, be used by the different rendering threads
	for (uint32 i = 0; i < 6; ++i) {
		mFreeRenderContexts.push_back(mActiveWindow->GetRenderContext()->CreateRenderContext());
	}

	//
	// Register kernel specific resource loaders
	//

	mResourceManager->RegisterLoader(new OpenALMusicResourceLoader(), "ogg");
	mResourceManager->RegisterLoader(new OpenALSoundEffectResourceLoader(), "wav");

	return true;
}

void Win32Kernel::Release()
{
	delete mThreadPool;
	delete mResourceManager;
	delete mActiveWindow;
	delete mInputDevices;
	delete mSoundEngine;
	delete mConfiguration;
	delete mScriptManager;
	delete mFileSystem;
	delete mLogger;
}

IConfiguration* Win32Kernel::GetConfiguration()
{
	return mConfiguration;
}

IFileSystem* Win32Kernel::GetFileSystem()
{
	return mFileSystem;
}

IInputDevices* Win32Kernel::GetInputDevices()
{
	return mInputDevices;
}

IGame* Win32Kernel::GetGame()
{
	return mGame;
}

ILogger* Win32Kernel::GetLogger()
{
	return mLogger;
}

IResourceManager* Win32Kernel::GetResourceManager()
{
	return mResourceManager;
}

IScriptManager* Win32Kernel::GetScriptManager()
{
	return mScriptManager;
}

IClassLoader* Win32Kernel::GetClassLoader()
{
	return mScriptManager;
}

ISoundEngine* Win32Kernel::GetSoundEngine()
{
	return mSoundEngine;
}

IActiveWindow* Win32Kernel::GetActiveWindow()
{
	return mActiveWindow;
}

IRenderContext* Win32Kernel::GetRenderContext()
{
	IRenderContext* renderContext = Win32RenderContext::GetThreadLocal();
	if (renderContext == nullptr) {
		renderContext = GetFreeRenderContext();
		Win32RenderContext::SetThreadLocal(renderContext);
	}
	return renderContext;
}

ThreadPool* Win32Kernel::GetThreadPool()
{
	return mThreadPool;
}

void Win32Kernel::StartGame(IGame* game)
{
	assert_not_null(game);
	assert_null(mGame);

	mGame = game;
	try {
		mGame->Start(this);
	}
	catch (Exception e) {
		mActiveWindow->Alert("Unhandled exception", e, AlertType::ALERT_ERROR);
	}
	delete game;
	mGame = nullptr;
}

bool Win32Kernel::ProcessEvents()
{
	mSoundEngine->UpdateStreamingBuffers();
	mScriptManager->GC();
	mFileSystem->ProcessEvents();
	return mActiveWindow->ProcessEvents();
}

OpenGLRenderContext* Win32Kernel::GetFreeRenderContext()
{
	std::lock_guard<std::mutex> lock(mFreeRenderContextsMutex);
	assert(mFreeRenderContexts.size() > 0 && "You don't have any more render contexts. Increase the number of render context in the kernels init phase to get more.");
	OpenGLRenderContext* renderContext = mFreeRenderContexts.front();
	mFreeRenderContexts.pop_front();
	return renderContext;
}

IEventDrivenKernel* core::CreateKernel(void* params)
{
	return new Win32Kernel();
}

void core::DestroyKernel(IEventDrivenKernel* kernel)
{
	delete kernel;
}
