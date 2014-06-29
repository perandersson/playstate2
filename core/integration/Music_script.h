#pragma once
#include "../sound/SoundEngine.h"
#include "../script/ScriptUtils.h"

int Music_Play(struct lua_State* L);
int Music_FadeInPlay(struct lua_State* L);
int Music_Stop(struct lua_State* L);
int Music_FadeOutStop(struct lua_State* L);

static luaL_Reg Music_Methods[] = {
	{ "Play", Music_Play },
	{ "FadeInPlay", Music_FadeInPlay },
	{ "Stop", Music_Stop },
	{ "FadeOutStop", Music_FadeOutStop },
	{ nullptr, nullptr }
};
