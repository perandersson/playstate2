#pragma once
#include "../scene/LinkedListSceneGroup.h"
#include "../script/ScriptUtils.h"

int LinkedListSceneGroup_Factory(struct lua_State* L);
int LinkedListSceneGroup_Init(struct lua_State* L);

static luaL_Reg LinkedListSceneGroup_Methods[] = {
	{ SCRIPT_CONSTRUCTOR, LinkedListSceneGroup_Factory },
	{ SCRIPT_INHERIT_CONSTRUCTOR, LinkedListSceneGroup_Init },
	{ nullptr, nullptr }
};
