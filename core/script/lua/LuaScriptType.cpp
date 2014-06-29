#include "../../Memory.h"
#include "LuaScriptType.h"
#include "LuaScriptManager.h"
#include "../ScriptObject.h"
#include "../LuaStateScopedLock.h"
using namespace core;

LuaScriptType::LuaScriptType(LuaScriptManager* scriptManager, const std::string& name)
: mScriptManager(scriptManager), mName(name)
{

}

LuaScriptType::~LuaScriptType()
{

}

const std::string& LuaScriptType::GetName() const
{
	return mName;
}

IScriptType& LuaScriptType::AddFunction(const char* name, lua_CFunction function)
{
	std::string nameAsString(name);
	Functions::iterator it = mFunctions.find(nameAsString);
	if (it != mFunctions.end()) {
		mFunctions.erase(it);
	}
	mFunctions.insert(std::make_pair(std::string(name), function));
	return *this;
}

IScriptType& LuaScriptType::AddFunctions(luaL_Reg* methods)
{
	for (; methods->name != nullptr; methods++) {
		AddFunction(methods->name, methods->func);
	}

	return *this;
}

IScriptType& LuaScriptType::InheritFrom(const char* name)
{
	return InheritFrom(ClassLoader::GetType(name));
}

IScriptType& LuaScriptType::InheritFrom(const IScriptType& type)
{
	// Only copy those functions that's not already added to the functions map for this type
	const LuaScriptType& t = (const LuaScriptType&)type;
	const Functions& functions = t.mFunctions;
	Functions::const_iterator tend = functions.end();
	for (Functions::const_iterator it = functions.begin(); it != tend; ++it) {
		if (mFunctions.find(it->first) == mFunctions.end()) {
			mFunctions.insert(std::make_pair(it->first, it->second));
		}
	}

	return *this;
}

uint32 LuaScriptType::RegisterObject(ScriptObject* object) const
{
	LuaStateScopedLock lock(mScriptManager);
	lua_State* L = lock.GetLuaState();

	// Create weak reference container
	lua_newtable(L);
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");

	const std::string metaName = mName + "_mt";
	luaL_getmetatable(L, metaName.c_str());
	lua_setmetatable(L, -2);

	// Set pointer to the container 
	lua_pushstring(L, ScriptObjectRefID);
	lua_pushlightuserdata(L, object);
	lua_settable(L, -3);

	// Set pointer to the container 
	lua_pushstring(L, ScriptObjectTypeID);
	lua_pushstring(L, mName.c_str());
	lua_settable(L, -3);

	// Pop the table from the lua state and return the lua ref
	return luaL_ref(L, LUA_REGISTRYINDEX);
}

uint32 LuaScriptType::RegisterObject(ScriptObject* object, uint32 scriptID) const
{
	LuaStateScopedLock lock(mScriptManager);
	lua_State* L = lock.GetLuaState();

	// Set the object to the weak reference supplied to this method
	lua_rawgeti(L, LUA_REGISTRYINDEX, scriptID);

	// Set pointer to the container 
	lua_pushstring(L, ScriptObjectRefID);
	lua_pushlightuserdata(L, object);
	lua_settable(L, -3);

	// Set pointer to the container 
	lua_pushstring(L, ScriptObjectTypeID);
	lua_pushstring(L, mName.c_str());
	lua_settable(L, -3);

	// Pop the table from the lua state and return the lua ref
	lua_pop(L, 1);
	return scriptID;
}

void LuaScriptType::UnregisterObject(ScriptObject* object) const
{
	// First bind the weak reference container and set the pointer to nil.
	LuaStateScopedLock lock(mScriptManager);
	lua_State* L = lock.GetLuaState();

	lua_rawgeti(L, LUA_REGISTRYINDEX, object->GetScriptID());
	lua_pushstring(L, ScriptObjectRefID);
	lua_pushnil(L);
	lua_settable(L, -3);

	lua_pushstring(L, ScriptObjectTypeID);
	lua_pushnil(L);
	lua_settable(L, -3);

	// Then release the container table
	lua_pop(L, 1);
	luaL_unref(L, LUA_REGISTRYINDEX, object->GetScriptID());
}

void LuaScriptType::Build()
{
	LuaStateScopedLock lock(mScriptManager);
	lua_State* L = lock.GetLuaState();

	const std::string metaName = mName + "_mt";

	luaL_newmetatable(L, metaName.c_str());
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	Functions::const_iterator end = mFunctions.end();
	for (Functions::iterator it = mFunctions.begin(); it != end; ++it) {
		lua_pushcclosure(L, it->second, 0);
		lua_setfield(L, -2, it->first.c_str());
	}
	lua_pop(L, 1);

	lua_newtable(L);
	luaL_getmetatable(L, metaName.c_str());
	lua_setmetatable(L, -2);
	for (Functions::iterator it = mFunctions.begin(); it != end; ++it) {
		lua_pushcclosure(L, it->second, 0);
		lua_setfield(L, -2, it->first.c_str());
	}
	lua_setglobal(L, mName.c_str());
}
