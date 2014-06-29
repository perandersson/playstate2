#pragma once
#include "../window/ActiveWindow.h"
#include "../script/ScriptUtils.h"

//
// Script integration
//

int ActiveWindow_GetWidth(struct lua_State* L);
int ActiveWindow_GetHeight(struct lua_State* L);
int ActiveWindow_GetSize(struct lua_State* L);

static luaL_Reg ActiveWindow_Methods[] = {
	{ "GetWidth", ActiveWindow_GetWidth },
	{ "GetHeight", ActiveWindow_GetHeight },
	{ "GetSize", ActiveWindow_GetSize },
	{ nullptr, nullptr }
};
