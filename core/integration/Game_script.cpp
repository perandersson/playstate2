#include "../Memory.h"
#include "Game_script.h"
using namespace core;

int Game_SetRenderPipeline(lua_State* L)
{
	assert_not_implemented();
	IRenderPipeline* renderPipeline = nullptr;

	Game::SetRenderPipeline(renderPipeline);
	return 0;
}

int Game_Stop(lua_State* L)
{
	Game::Stop();
	return 0;
}
