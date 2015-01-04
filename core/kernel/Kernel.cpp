#include "../Memory.h"
#include "Kernel.h"
using namespace core;

template<> IKernel* Singleton<IKernel>::gSingleton = nullptr;

IConfiguration* Kernel::GetConfiguration()
{
	return GetInstance()->GetConfiguration();
}

IFileSystem* Kernel::GetFileSystem()
{
	return GetInstance()->GetFileSystem();
}

IInputDevices* Kernel::GetInputDevices()
{
	return GetInstance()->GetInputDevices();
}

IGame* Kernel::GetGame()
{
	return GetInstance()->GetGame();
}

ILogger* Kernel::GetLogger()
{
	return GetInstance()->GetLogger();
}

IResourceManager* Kernel::GetResourceManager()
{
	return GetInstance()->GetResourceManager();
}

IScriptManager* Kernel::GetScriptManager()
{
	return GetInstance()->GetScriptManager();
}

IClassLoader* Kernel::GetClassLoader()
{
	return GetInstance()->GetClassLoader();
}

ISoundEngine* Kernel::GetSoundEngine()
{
	return GetInstance()->GetSoundEngine();
}

IActiveWindow* Kernel::GetActiveWindow()
{
	return GetInstance()->GetActiveWindow();
}

IRenderContext* Kernel::GetRenderContext()
{
	return GetInstance()->GetRenderContext();
}

ThreadPool* Kernel::GetThreadPool()
{
	return GetInstance()->GetThreadPool();
}

void Kernel::StartGame(IGame* game)
{
	GetInstance()->StartGame(game);
}
