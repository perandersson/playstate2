#include "../Memory.h"
#include "Game.h"
#include "../kernel/Kernel.h"
using namespace core;

void Game::Stop()
{
	Kernel::GetGame()->Stop();
}

void Game::SetRenderPipeline(IRenderPipeline* renderPipeline)
{
	Kernel::GetGame()->SetRenderPipeline(renderPipeline);
}

Scene* Game::GetScene()
{
	return Kernel::GetGame()->GetScene();
}
