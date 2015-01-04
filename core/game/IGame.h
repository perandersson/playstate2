#pragma once
#include "../resource/IResourceManager.h"
#include "../rendering/IRenderPipeline.h"
#include "../rendering/IRenderContext.h"
#include "../scene/Scene.h"

namespace core
{
	class IEventDrivenKernel;

	/*!
		\brief The IGame interface is used by the game engine as the main entry-point of the game.
	*/
	class IGame
	{
	public:
		virtual ~IGame() {}


		/*!
			\brief Method which starts the game
		*/
		virtual void Start(IEventDrivenKernel* kernel) = 0;

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


}
