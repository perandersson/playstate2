#pragma once
#include "../script/ScriptUtils.h"
#include "KeyboardDevice_script.h"
#include "MouseDevice_script.h"

//
// Script integration
//

int InputDevices_GetMouseDevice(struct lua_State* L);
int InputDevices_GetMouseDevices(struct lua_State* L);
int InputDevices_GetKeyboardDevice(struct lua_State* L);
int InputDevices_GetKeyboardDevices(struct lua_State* L);

static luaL_Reg InputDevices_Methods[] = {
	{ "GetMouseDevice", InputDevices_GetMouseDevice },
	{ "GetMouseDevices", InputDevices_GetMouseDevices },
	{ "GetKeyboardDevice", InputDevices_GetKeyboardDevice },
	{ "GetKeyboardDevices", InputDevices_GetKeyboardDevices },
	{ nullptr, nullptr }
};
