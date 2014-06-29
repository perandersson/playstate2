#pragma once
#include "../resource/ResourceManager.h"
#include "../script/ScriptUtils.h"

int Resource_Load(struct lua_State* L);

static luaL_Reg Resource_Methods[] = {
	{ "Load", Resource_Load },
	{ nullptr, nullptr }
};
