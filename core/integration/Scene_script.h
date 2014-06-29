#pragma once
#include "../script/ScriptUtils.h"
#include "../scene/Scene.h"

int Scene_GetActiveCamera(struct lua_State* L);
int Scene_SetActiveCamera(struct lua_State* L);
int Scene_SetAmbientLight(struct lua_State* L);
int Scene_GetAmbientLight(struct lua_State* L);

static luaL_Reg Scene_Methods[] = {
	{ "GetActiveCamera", Scene_GetActiveCamera },
	{ "SetActiveCamera", Scene_SetActiveCamera },
	{ "GetAmbientLight", Scene_GetAmbientLight },
	{ "SetAmbientLight", Scene_SetAmbientLight },
	{ nullptr, nullptr }
};
