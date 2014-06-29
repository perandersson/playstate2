#include "../../Memory.h"
#include "LuaScriptEnum.h"
#include "LuaScriptManager.h"
#include "../LuaStateScopedLock.h"
using namespace core;

LuaScriptEnum::LuaScriptEnum(LuaScriptManager* scriptManager, const std::string& name)
: mScriptManager(scriptManager), mName(name)
{

}

LuaScriptEnum::~LuaScriptEnum()
{

}

const std::string& LuaScriptEnum::GetName() const
{
	return mName;
}

IScriptEnum& LuaScriptEnum::Enum(const char* name, int32 value)
{
	mEnums.insert(std::make_pair(std::string(name), value));
	return *this;
}

IScriptEnum& LuaScriptEnum::Enums(const std::hash_map<std::string, int32>& enums)
{
	mEnums.insert(enums.begin(), enums.end());
	return *this;
}

void LuaScriptEnum::Build()
{
	LuaStateScopedLock lock(mScriptManager);
	lua_State* L = lock.GetLuaState();

	lua_newtable(L);
	EnumMap::const_iterator end = mEnums.end();
	for (EnumMap::iterator it = mEnums.begin(); it != end; ++it) {
		lua_pushinteger(L, it->second);
		lua_setfield(L, -2, it->first.c_str());
	}
	lua_setglobal(L, mName.c_str());
}
