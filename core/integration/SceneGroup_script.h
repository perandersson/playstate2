#pragma once
#include "../scene/SceneGroup.h"
#include "../script/ScriptUtils.h"

int SceneGroup_Factory(struct lua_State* L);
int SceneGroup_Init(struct lua_State* L);
int SceneGroup_AddSceneNode(struct lua_State* L);
int SceneGroup_RemoveSceneNode(struct lua_State* L);
int SceneGroup_SetPosition(struct lua_State* L);

static luaL_Reg SceneGroup_Methods[] = {
	{ SCRIPT_CONSTRUCTOR, SceneGroup_Factory },
	{ SCRIPT_INHERIT_CONSTRUCTOR, SceneGroup_Init },
	{ "AddSceneNode", SceneGroup_AddSceneNode },
	{ "RemoveSceneNode", SceneGroup_RemoveSceneNode },
	{ "SetPosition", SceneGroup_SetPosition },
	{ nullptr, nullptr }
};
