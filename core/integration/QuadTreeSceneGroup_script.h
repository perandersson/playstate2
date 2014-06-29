#pragma once
#include "../scene/QuadTreeSceneGroup.h"
#include "../script/ScriptUtils.h"

int QuadTreeSceneGroup_Factory(struct lua_State* L);
int QuadTreeSceneGroup_Init(struct lua_State* L);
int QuadTreeSceneGroup_SetQuadTree(struct lua_State* L);

static luaL_Reg QuadTreeSceneGroup_Methods[] = {
	{ SCRIPT_CONSTRUCTOR, QuadTreeSceneGroup_Factory },
	{ SCRIPT_INHERIT_CONSTRUCTOR, QuadTreeSceneGroup_Init },
	{ "SetQuadTree", QuadTreeSceneGroup_SetQuadTree },
	{ nullptr, nullptr }
};
