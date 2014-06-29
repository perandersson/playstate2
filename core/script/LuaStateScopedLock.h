#pragma once

extern "C"
{
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}

namespace core
{
	class IScriptManager;

	class LuaStateScopedLock
	{
	public:
		LuaStateScopedLock(IScriptManager* scriptManager);
		~LuaStateScopedLock();

		//
		// Return the current lua state
		lua_State* GetLuaState();

	private:
		IScriptManager* mScriptManager;
		lua_State* mLuaState;
	};

}
