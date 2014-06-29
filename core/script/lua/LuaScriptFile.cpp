#include "../../Memory.h"
#include "LuaScriptFile.h"
#include "../LuaStateScopedLock.h"
#include "../exception/ScriptException.h"
using namespace core;

LuaScriptFile::LuaScriptFile(IScriptManager* scriptManager)
: mScriptManager(scriptManager)
{

}

LuaScriptFile::~LuaScriptFile()
{

}

ScriptResult LuaScriptFile::Execute(const char* expression)
{
	LuaStateScopedLock lock(mScriptManager);
	lua_State* L = lock.GetLuaState();
	
	int idxBefore = lua_gettop(L);
	int result = luaL_dostring(L, expression);
	if (result != 0) {
		std::string err = lua_tostring(L, -1);
		lua_pop(L, 1);
		THROW_EXCEPTION(ScriptException, "Could not evaluate expression:\n%s\n\nReason:\n%s", expression, err.c_str());
	}
	int numItemsPushedOnStack = lua_gettop(L) - idxBefore;
	return ScriptResult(mScriptManager, numItemsPushedOnStack);
}
