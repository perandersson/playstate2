#pragma once
#include "../IEventDrivenKernel.h"
#include "../../configuration/lua/LuaConfiguration.h"
#include "../../filesystem/win32/Win32FileSystem.h"
#include "../../input/win32/Win32InputDevices.h"
#include "../../logging/console/StdCoutLogger.h"
#include "../../resource/threadsafe/ThreadSafeResourceManager.h"
#include "../../sound/openal/OpenALSoundEngine.h"
#include "../../script/lua/LuaScriptManager.h"
#include "../../window/win32/Win32ActiveWindow.h"

namespace core
{
	/*!
		\brief Standard win32 kernel used by the game engine
	*/
	class Win32Kernel : public IEventDrivenKernel
	{
		typedef std::list<OpenGLRenderContext*> FreeRenderContexts;
	public:
		Win32Kernel();
		virtual ~Win32Kernel();

		virtual bool Initialize();
		virtual void Release();
		virtual IConfiguration* GetConfiguration();
		virtual IFileSystem* GetFileSystem();
		virtual IInputDevices* GetInputDevices();
		virtual IGame* GetGame();
		virtual ILogger* GetLogger();
		virtual IResourceManager* GetResourceManager();
		virtual IScriptManager* GetScriptManager();
		virtual IClassLoader* GetClassLoader();
		virtual ISoundEngine* GetSoundEngine();
		virtual IActiveWindow* GetActiveWindow();
		virtual IRenderContext* GetRenderContext();
		virtual ThreadPool* GetThreadPool();
		virtual void StartGame(IGame* game);
		virtual bool ProcessEvents();

	private:
		OpenGLRenderContext* GetFreeRenderContext();
		void ReturnRenderContext(OpenGLRenderContext* renderContext);

	private:
		LuaConfiguration* mConfiguration;
		Win32FileSystem* mFileSystem;
		Win32InputDevices* mInputDevices;
		IGame* mGame;
		StdCoutLog* mLogger;
		ThreadSafeResourceManager* mResourceManager;
		LuaScriptManager* mScriptManager;
		OpenALSoundEngine* mSoundEngine;
		Win32ActiveWindow* mActiveWindow;
		ThreadPool* mThreadPool;

		// List containing all the free render contexts
		FreeRenderContexts mFreeRenderContexts;
		std::mutex mFreeRenderContextsMutex;
	};
}
