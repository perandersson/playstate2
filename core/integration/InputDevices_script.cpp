#include "../Memory.h"
#include "InputDevices_script.h"
#include "../input/InputDevices.h"
using namespace core;

int InputDevices_GetMouseDevice(struct lua_State* L)
{
	const uint32 idx = ScriptUtils::ToUInt32(L);
	const IMouseDevice* mouseDevice = InputDevices::GetMouseDevice(idx);
	if (mouseDevice == nullptr) {
		lua_pushnil(L);
		return 1;
	}

	return ScriptUtils::PushObject(L, new ScriptMouseDevice(mouseDevice));
}

int InputDevices_GetMouseDevices(struct lua_State* L)
{
	assert_not_implemented();
	return 0;

}

int InputDevices_GetKeyboardDevice(struct lua_State* L)
{
	const uint32 idx = ScriptUtils::ToUInt32(L);
	const IKeyboardDevice* keyboardDevice = InputDevices::GetKeyboardDevice(idx);
	if (keyboardDevice == nullptr) {
		lua_pushnil(L);
		return 1;
	}

	return ScriptUtils::PushObject(L, new ScriptKeyboardDevice(keyboardDevice));
}

int InputDevices_GetKeyboardDevices(struct lua_State* L)
{
	assert_not_implemented();
	return 0;

}


