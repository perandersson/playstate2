#pragma once
#include "../input/IKeyboardDevice.h"
#include "../script/ScriptObject.h"

namespace core
{
	//
	// Wraps the keyboard device into a scriptable object so that it's available from script
	class ScriptKeyboardDevice : public IKeyboardDevice, public ScriptObject
	{
		DECLARE_SCRIPT_OBJECT(KeyboardDevice);

	public:
		ScriptKeyboardDevice(const IKeyboardDevice* keyboardDevice);
		virtual ~ScriptKeyboardDevice();

	// IKeyboardDevice
	public:
		virtual uint32 GetIndex() const;
		virtual bool IsDown(KeyboardKey::Enum button) const;
		virtual bool IsUp(KeyboardKey::Enum button) const;

	private:
		const IKeyboardDevice* mKeyboardDevice;
	};
}

//
// Script integration
//

int KeyboardDevice_GC(struct lua_State* L);
int KeyboardDevice_GetIndex(struct lua_State* L);
int KeyboardDevice_IsUp(struct lua_State* L);
int KeyboardDevice_IsDown(struct lua_State* L);

static luaL_Reg KeyboardDevice_Methods[] = {
	{ SCRIPT_GC, KeyboardDevice_GC },
	{ "GetIndex", KeyboardDevice_GetIndex },
	{ "IsUp", KeyboardDevice_IsUp },
	{ "IsDown", KeyboardDevice_IsDown },
	{ nullptr, nullptr }
};
