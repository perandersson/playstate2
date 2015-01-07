#pragma once
#include <core/kernel/IKernel.h>

#include <core/configuration/lua/LuaConfiguration.h>
#include <core/filesystem/win32/Win32FileSystem.h>
#include <core/input/win32/Win32InputDevices.h>
#include <core/logging/console/StdCoutLogger.h>
#include <core/resource/threadsafe/ThreadSafeResourceManager.h>
#include <core/sound/openal/OpenALSoundEngine.h>
#include <core/script/lua/LuaScriptManager.h>
#include <core/window/win32/Win32ActiveWindow.h>

namespace core
{
	/*!
		\brief Standard win32 kernel used by the game engine
	*/
	class Win32Kernel : public IKernel
	{
		typedef std::list<OpenGLRenderContext*> FreeRenderContexts;
	public:
		Win32Kernel();
		virtual ~Win32Kernel();

		/*!
			\brief Initialize the windows kernel
		*/
		bool Initialize();

		/*!
			\brief Release the windows kernel
		*/
		void Release();

		/*!
			\brief Process this kernels internal events
		*/
		bool ProcessEvents();

		/*!
			\brief Start the supplied game
		*/
		void StartGame(IGame* game);

	public:
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
