#pragma once
#include "IKernel.h"
#include "../Singleton.h"

namespace core
{
	class Kernel : public Singleton<IKernel>
	{
	public:
		static IConfiguration* GetConfiguration();
		static IFileSystem* GetFileSystem();
		static IInputDevices* GetInputDevices();
		static IGame* GetGame();
		static ILogger* GetLogger();
		static IResourceManager* GetResourceManager();
		static IScriptManager* GetScriptManager();
		static IClassLoader* GetClassLoader();
		static ISoundEngine* GetSoundEngine();
		static IActiveWindow* GetActiveWindow();
		static IRenderContext* GetRenderContext();
		static ThreadPool* GetThreadPool();
	};
}
