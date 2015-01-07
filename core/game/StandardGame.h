#pragma once
#include "Game.h"
#include "../filesystem/IFileChangedListener.h"

namespace core
{
	/*!
		\brief Standard game implementation
		
		Create your own game instance by inherit this class. It handles the basic update, rendering and scene management
		for your game.
	*/
	class StandardGame : public IGame, public IFileChangedListener
	{
	public:
		/*!
			\brief The constructor of the game requires a pointer to the kernel. 
			
			It is from the kernel where you can get the services used by the game engine to process the game

			\param kernel
		*/
		StandardGame(IKernel* kernel);
		virtual ~StandardGame();

	// IGame
	public:
		virtual void Start();
		virtual bool Process(float64 dt);
		virtual void Stop();
		virtual bool Initialize();
		virtual void Release();
		virtual void Update();
		virtual void Render();
		virtual void LoadContent(IResourceManager* resourceManager);
		virtual void UnloadContent(IResourceManager* resourceManager);
		virtual Scene* GetScene();
		virtual void SetRenderPipeline(IRenderPipeline* pipeline);

	// IFileChangedListener
	public:
		virtual void OnFileChanged(const IFile* file, FileChangeAction::Enum action);
		
	protected:
		IKernel* mKernel;

	private:
		bool mRunning;
		Scene mScene;
		IRenderPipeline* mRenderPipeline;
	};
}
