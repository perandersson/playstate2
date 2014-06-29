#pragma once
#include "../sound/SoundEngine.h"
#include "../script/ScriptUtils.h"

//
// Script integration
//

int SoundEngine_GetSoundReceiver(struct lua_State* L);

static luaL_Reg SoundEngine_Methods[] = {
	{ "GetSoundReceiver", SoundEngine_GetSoundReceiver },
	{ nullptr, nullptr }
};
