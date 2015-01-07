#pragma once
#include "../resource/IResourceManager.h"
#include "../rendering/IRenderPipeline.h"
#include "../rendering/IRenderContext.h"
#include "../scene/Scene.h"

namespace core
{
	class IKernel;

	/*!
		\brief The IGame interface is used by the game engine as the main entry-point of the game.
	*/
	class IGame
	{
	public:
		virtual ~IGame() {}

		/*!
			\brief Start the game
		*/
		virtual void Start() = 0;

		/*!
			\brief Process the game's internal functionality

			\param dt
					Time since last update
			\return TRUE if we want to continue processing the game
		*/
		virtual bool Process(float64 dt) = 0;

		/*!
			\brief Stops the game from running
		*/
		virtual void Stop() = 0;

		/*!
			\brief Initializes the game's internal resources.

			\return TRUE if the initialization was successfull; FALSE otherwise
		*/
		virtual bool Initialize() = 0;

		/*!
			\brief Releases the game's internal resources.
		*/
		virtual void Release() = 0;

		/*!
			\brief Update the game
		*/
		virtual void Update() = 0;

		/*!
			\brief Render this game
		*/
		virtual void Render() = 0;
		
		/*!
			\brief Load content related to the actual game

			\param resourceManager
						The resource manager used to load resources from the file system
		*/
		virtual void LoadContent(IResourceManager* resourceManager) = 0;

		/*!
			\brief Unloads content

			\param resourceManager
						The resource manager used to load resources from the file system
		*/
		virtual void UnloadContent(IResourceManager* resourceManager) = 0;

		/*!
			\brief Retrieves the game's scene

			\return The game's scene
		*/
		virtual Scene* GetScene() = 0;

		/*!
			\brief Set's the rendering pipeline used by the game to draw the scene

			\param pipeline
						The pipeline new pipeline responsible for drawing the scene
		*/
		virtual void SetRenderPipeline(IRenderPipeline* pipeline) = 0;
	};

	class IGameFactory {
	public:
		virtual ~IGameFactory() {}

		/*!
			\brief Creates the game instance
		*/
		virtual IGame* Create(IKernel* kernel) = 0;
	};

	/* Pointer to the factory responsible for creating the game */
	extern IGameFactory* gGameFactory;
}

#ifndef DECLARE_GAME
#define DECLARE_GAME(Class) \
	class Class##_Factory : public core::IGameFactory { \
	public: \
		Class##_Factory() { core::gGameFactory = this; } \
		virtual ~Class##_Factory() {} \
		virtual IGame* Create(core::IKernel* kernel) { return new Class(kernel); } \
	} g##Class##GameFactory
#endif
