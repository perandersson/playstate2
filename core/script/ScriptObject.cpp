#include "../Memory.h"
#include "ScriptObject.h"
#include "IClassLoader.h"
#include "LuaStateScopedLock.h"
#include "../kernel/Kernel.h"
using namespace core;


ScriptObject::ScriptObject()
: mScriptID(0)
{
}

ScriptObject::~ScriptObject()
{
	UnregisterObject();
}

bool ScriptObject::RegisterObject()
{
	return RegisterObject(GetScriptType()->RegisterObject(this));
}

bool ScriptObject::RegisterObject(uint32 scriptID)
{
	mScriptID = scriptID;

	if (!OnRegisterObject() && mScriptID != 0) {
		UnregisterObject();
		mScriptID = 0;
		return false;
	}

	return true;
}

void ScriptObject::UnregisterObject()
{
	if (mScriptID == 0)
		return;

	// Invoke callbacks before detaching this object from script
	OnUnregisterObject();

	// Release the script reference
	GetScriptType()->UnregisterObject(this);

	// Release the script reference
	mScriptID = 0;
}

bool ScriptObject::OnRegisterObject()
{
	return true;
}

void ScriptObject::OnUnregisterObject()
{
}

std::shared_ptr<ScriptMethod> ScriptObject::GetMethodPtr(const char* name)
{
	// First bind the weak reference container and set the pointer to nil.
	IScriptManager* scriptManager = Kernel::GetScriptManager();
	LuaStateScopedLock lock(scriptManager);
	lua_State* L = lock.GetLuaState();

	lua_rawgeti(L, LUA_REGISTRYINDEX, mScriptID);
	if (lua_istable(L, -1)) {
		lua_getfield(L, -1, name);
		if (lua_isfunction(L, -1)) {
			lua_remove(L, -2); // Only the reference to the method exists on the stack after this
			const uint32 ref = luaL_ref(L, LUA_REGISTRYINDEX);
			return std::shared_ptr<ScriptMethod>(new ScriptMethod(scriptManager, mScriptID, ref));
		}
		lua_pop(L, 1);
	}
	lua_pop(L, 1);
	return std::shared_ptr<ScriptMethod>(new ScriptMethod(scriptManager, mScriptID, 0));
}
