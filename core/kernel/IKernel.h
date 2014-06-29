#pragma once
#include "../configuration/IConfiguration.h"
#include "../filesystem/IFileSystem.h"
#include "../input/IInputDevices.h"
#include "../game/IGame.h"
#include "../logging/ILogger.h"
#include "../resource/IResourceManager.h"
#include "../sound/ISoundEngine.h"
#include "../window/IActiveWindow.h"
#include "../script/IClassLoader.h"
#include "../rendering/IRenderContext.h"
#include "ThreadPool.h"

namespace core
{
	class IKernel
	{
	public:
		virtual ~IKernel() {}

		/*!
			\brief Initialize the kernel. 
		*/
		virtual bool Initialize() = 0;

		/*!
			\brief Releases this kernels internal resources
		*/
		virtual void Release() = 0;

		/*!
			\brief Retrieves the configuration manager
		*/
		virtual IConfiguration* GetConfiguration() = 0;

		/*!
			\brief Retrieves the filesystem used by the game engine
		*/
		virtual IFileSystem* GetFileSystem() = 0;

		/*!
			\brief Retrieves the input devices usable by the game engine
		*/
		virtual IInputDevices* GetInputDevices() = 0;

		/*!
			\brief Retrieves the game instance running the actual game
		*/
		virtual IGame* GetGame() = 0;

		/*!
			\brief Retrieves the logger
		*/
		virtual ILogger* GetLogger() = 0;

		/*!
			\brief Retrieves the resource manager
		*/
		virtual IResourceManager* GetResourceManager() = 0;

		/*!
			\brief Retrieves the script manager used by the game engine
		*/
		virtual IScriptManager* GetScriptManager() = 0;

		/*!
			\brief Retrieves the classloader
		*/
		virtual IClassLoader* GetClassLoader() = 0;

		/*!
			\brief Retrieves the sound engine used by the game engine to play sound and music
		*/
		virtual ISoundEngine* GetSoundEngine() = 0;

		/*!
			\brief Retrieves the active window where the actual game is drawn into
		*/
		virtual IActiveWindow* GetActiveWindow() = 0;

		/*!
			\brief Retrieves the render context for the current thread

			If no render context exists for the current thread then a new one will
			be created. Resources created by the new context is automatically transferable
			between other render contexts because all render contexts inherit from the main render context.

			\return A pointer to the render context 
		*/
		virtual IRenderContext* GetRenderContext() = 0;

		/*!
			\brief
		*/
		virtual ThreadPool* GetThreadPool() = 0;

		/*!
			\brief Start the supplied game instance
		*/
		virtual void StartGame(IGame* game) = 0;

		/*!
			\brief Process the internal kernel events.

			If the kernel has decided to stop processing it's internal events it usually means that the game window has been closed or
			if the game has been closed by an external party (i.e. the operating system)

			\return TRUE if the kernel proccessed the events correctly; FALSE otherwise. 
		*/
		virtual bool ProcessEvents() = 0;
	};

	/*!
		\brief Create a kernel instance. 
		
		\param params
					Extra parameters
		\return A kernel instance
	*/
	extern IKernel* CreateKernel(void* params);

	/*!
		\brief Destroy the supplied kernel
		
		\param kernel
					The kernel we want to destroy
	*/
	extern void DestroyKernel(IKernel* kernel);
}


