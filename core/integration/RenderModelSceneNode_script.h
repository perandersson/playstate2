#pragma once
#include "../scene/rendering/RenderModelSceneNode.h"
#include "../script/ScriptUtils.h"

int RenderModelSceneNode_Factory(struct lua_State* L);
int RenderModelSceneNode_Init(struct lua_State* L);
int RenderModelSceneNode_SetModel(struct lua_State* L);

static luaL_Reg RenderModelSceneNode_Methods[] = {
	{ SCRIPT_CONSTRUCTOR, RenderModelSceneNode_Factory },
	{ SCRIPT_INHERIT_CONSTRUCTOR, RenderModelSceneNode_Init },
	{ "SetModel", RenderModelSceneNode_SetModel },
	{ nullptr, nullptr }
};
