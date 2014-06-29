#pragma once
#include "../script/ScriptUtils.h"
#include "../game/Game.h"

int Game_SetRenderPipeline(struct lua_State* L);
int Game_Stop(struct lua_State* L);

static luaL_Reg Game_Methods[] = {
	{ "SetRenderPipeline", Game_SetRenderPipeline },
	{ "Stop", Game_Stop },
	{ nullptr, nullptr }
};
