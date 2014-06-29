#pragma once
#include "../sound/SoundEngine.h"
#include "../script/ScriptUtils.h"

//
// Script integration
//

int SoundEffect_Play(struct lua_State* L);
int SoundEffect_GetDuration(struct lua_State* L);

static luaL_Reg SoundEffect_Methods[] = {
	{ "Play", SoundEffect_Play },
	{ "GetDuration", SoundEffect_GetDuration },
	{ nullptr, nullptr }
};
