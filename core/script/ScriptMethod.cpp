#include "../Memory.h"
#include "ScriptMethod.h"
#include "LuaStateScopedLock.h"
using namespace core;

ScriptMethod::ScriptMethod(IScriptManager* scriptManager, uint32 scriptID, uint32 methodID)
: mScriptManager(scriptManager), mScriptID(scriptID), mMethodID(methodID)
{
	assert(scriptID != 0 && "Invalid scriptID");

}

ScriptMethod::~ScriptMethod()
{
	if (mMethodID != 0) {
		LuaStateScopedLock lock(mScriptManager);
		lua_State* L = lock.GetLuaState();

		luaL_unref(L, LUA_REGISTRYINDEX, mMethodID);
		mMethodID = 0;
	}
}

ScriptResult ScriptMethod::Invoke()
{
	if (mMethodID == 0)
		return ScriptResult(mScriptManager, 0);

	LuaStateScopedLock lock(mScriptManager);
	lua_State* L = lock.GetLuaState(); 

	int32 beforeCall = lua_gettop(L);
	// Try to bind the method to the current lua state
	if (PrepareMethod(L)) {
		if (lua_pcall(L, 1, LUA_MULTRET, NULL) == 0) {
			int32 afterCall = lua_gettop(L);
			return ScriptResult(mScriptManager, afterCall - beforeCall);
		}
		else {
			const std::string message = ScriptUtils::ToString(L);
			ScriptUtils::LogError(L, message.c_str());
		}
	}
	return ScriptResult(mScriptManager, 0);
}

bool ScriptMethod::PrepareMethod(struct lua_State* L)
{
	lua_rawgeti(L, LUA_REGISTRYINDEX, mMethodID);
	if (lua_isfunction(L, -1)) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, mScriptID);
		return true;
	}
	lua_pop(L, 1);
	return false;
}
