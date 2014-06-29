#include "../Memory.h"
#include "KeyboardDevice_script.h"
#include "../input/InputDevices.h"
using namespace core;

ScriptKeyboardDevice::ScriptKeyboardDevice(const IKeyboardDevice* keyboardDevice)
: ScriptObject(), mKeyboardDevice(keyboardDevice)
{

}

ScriptKeyboardDevice::~ScriptKeyboardDevice()
{

}

uint32 ScriptKeyboardDevice::GetIndex() const
{
	return mKeyboardDevice->GetIndex();
}

bool ScriptKeyboardDevice::IsDown(KeyboardKey::Enum button) const
{
	return mKeyboardDevice->IsDown(button);
}

bool ScriptKeyboardDevice::IsUp(KeyboardKey::Enum button) const
{
	return mKeyboardDevice->IsUp(button);
}

int KeyboardDevice_GC(struct lua_State* L)
{
	ScriptKeyboardDevice* self = ScriptUtils::ToObject<ScriptKeyboardDevice>(L);
	if (self != nullptr)
		delete self;

	return 0;
}

int KeyboardDevice_GetIndex(struct lua_State* L)
{
	ScriptKeyboardDevice* self = ScriptUtils::ToObject<ScriptKeyboardDevice>(L);
	if (self != nullptr)
		return ScriptUtils::PushUInt32(L, self->GetIndex());
	else
		ScriptUtils::LogError(L, "Expected: ScriptKeyboardDevice.GetIndex(self)");

	return ScriptUtils::PushUInt32(L, 0);
}

int KeyboardDevice_IsUp(struct lua_State* L)
{
	const KeyboardKey::Enum key = ScriptUtils::ToEnum<KeyboardKey::Enum>(L);
	ScriptKeyboardDevice* self = ScriptUtils::ToObject<ScriptKeyboardDevice>(L);
	if (self != nullptr)
		return ScriptUtils::PushBool(L, self->IsUp(key));
	else
		ScriptUtils::LogError(L, "Expected: ScriptKeyboardDevice.IsUp(self, KeyboardKey)");

	return ScriptUtils::PushBool(L, false);
}

int KeyboardDevice_IsDown(struct lua_State* L)
{
	const KeyboardKey::Enum key = ScriptUtils::ToEnum<KeyboardKey::Enum>(L);
	ScriptKeyboardDevice* self = ScriptUtils::ToObject<ScriptKeyboardDevice>(L);
	if (self != nullptr)
		return ScriptUtils::PushBool(L, self->IsDown(key));
	else
		ScriptUtils::LogError(L, "Expected: ScriptKeyboardDevice.IsDown(self, KeyboardKey)");
	
	return ScriptUtils::PushBool(L, false);	
}
