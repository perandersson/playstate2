#pragma once
#include "../input/IMouseDevice.h"
#include "../script/ScriptObject.h"

namespace core
{
	/*!
		\brief Script-wrapper class for the IMouseDevice interface
	*/
	class ScriptMouseDevice : public IMouseDevice, public ScriptObject
	{
		DECLARE_SCRIPT_OBJECT(MouseDevice);

	public:
		ScriptMouseDevice(const IMouseDevice* mouseDevice);
		virtual ~ScriptMouseDevice();

	// IMouseDevice
	public:
		virtual uint32 GetIndex() const;
		virtual bool IsDown(MouseButton::Enum button) const;
		virtual bool IsUp(MouseButton::Enum button) const;
		virtual Point GetPosition() const;

	private:
		const IMouseDevice* mMouseDevice;
	};
}

//
// LUA WRAPPERS
//

int MouseDevice_GC(struct lua_State* L);
int MouseDevice_GetIndex(struct lua_State* L);
int MouseDevice_IsUp(struct lua_State* L);
int MouseDevice_IsDown(struct lua_State* L);
int MouseDevice_GetPosition(struct lua_State* L);

static luaL_Reg MouseDevice_Methods[] = {
	{ SCRIPT_GC, MouseDevice_GC },
	{ "GetIndex", MouseDevice_GetIndex },
	{ "IsUp", MouseDevice_IsUp },
	{ "IsDown", MouseDevice_IsDown },
	{ "GetPosition", MouseDevice_GetPosition },
	{ nullptr, nullptr }
};
