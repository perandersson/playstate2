#pragma once
#include "Game.h"
#include "../filesystem/IFileChangedListener.h"
#include "../kernel/IKernel.h"

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
		StandardGame();
		virtual ~StandardGame();

	// IGame
	public:
		virtual void Start();
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
		
	private:
		bool mRunning;
		IKernel* mKernel;
		Scene mScene;
		IRenderPipeline* mRenderPipeline;
	};
}
