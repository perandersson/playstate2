#pragma once
#include "../camera/Camera.h"
#include "../script/ScriptUtils.h"

int Camera_Move(struct lua_State* L);
int Camera_LookAt(struct lua_State* L);
int Camera_GetPosition(struct lua_State* L);
int Camera_GetUp(struct lua_State* L);
int Camera_GetCenter(struct lua_State* L);

static luaL_Reg Camera_Methods[] = {
	{ "Move", Camera_Move },
	{ "LookAt", Camera_LookAt },
	{ "GetPosition", Camera_GetPosition },
	{ "GetUp", Camera_GetUp },
	{ "GetCenter", Camera_GetCenter },
	{ nullptr, nullptr }
};
