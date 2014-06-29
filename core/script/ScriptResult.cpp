#include "../Memory.h"
#include "ScriptResult.h"
#include "LuaStateScopedLock.h"
using namespace core;

ScriptResult::ScriptResult(const ScriptResult& rhs)
: mScriptManager(rhs.mScriptManager), mNumResults(rhs.mNumResults)
{
}

ScriptResult::ScriptResult(IScriptManager* scriptManager, uint32 numResults)
: mScriptManager(scriptManager), mNumResults(numResults)
{
}

ScriptResult::~ScriptResult()
{
	if (mScriptManager != nullptr && mNumResults > 0) {
		LuaStateScopedLock lock(mScriptManager);
		lua_State* L = lock.GetLuaState();

		lua_pop(L, (int32)mNumResults);
		mNumResults = 0;
	}

	mScriptManager = nullptr;
}

Table ScriptResult::ToTable(int32 idx) const
{
	if (mNumResults == 0)
		return Table(mScriptManager, 0);

	LuaStateScopedLock lock(mScriptManager);
	lua_State* L = lock.GetLuaState();

	if (!lua_istable(L, idx))
		return Table(mScriptManager, 0);

	int configRef = luaL_ref(L, LUA_REGISTRYINDEX);
	lua_rawgeti(L, LUA_REGISTRYINDEX, configRef);
	return Table(mScriptManager, configRef);
}

int32 ScriptResult::ToInt32(int32 idx) const
{
	if (mNumResults == 0)
		return 0;

	LuaStateScopedLock lock(mScriptManager);
	lua_State* L = lock.GetLuaState();

	return ScriptUtils::ToInt32(L, idx, false);
}

float32 ScriptResult::ToFloat32(int32 idx) const
{
	if (mNumResults == 0)
		return 0.0f;

	LuaStateScopedLock lock(mScriptManager);
	lua_State* L = lock.GetLuaState();

	return ScriptUtils::ToFloat32(L, idx, false);
}

bool ScriptResult::ToBool(int32 idx) const
{
	if (mNumResults == 0)
		return false;

	LuaStateScopedLock lock(mScriptManager);
	lua_State* L = lock.GetLuaState();

	return ScriptUtils::ToBool(L, idx, false);
}

std::string ScriptResult::ToString(int32 idx) const
{
	if (mNumResults == 0)
		return std::string();

	LuaStateScopedLock lock(mScriptManager);
	lua_State* L = lock.GetLuaState();

	return ScriptUtils::ToString(L, idx, false);
}

Color ScriptResult::ToColor(int32 idx) const
{
	if (mNumResults == 0)
		return Color::NOTHING;

	LuaStateScopedLock lock(mScriptManager);
	lua_State* L = lock.GetLuaState();

	return ScriptUtils::ToColor(L, idx, false);
}

Vector2 ScriptResult::ToVector2(int32 idx) const
{
	if (mNumResults == 0)
		return Vector2::ZERO;

	LuaStateScopedLock lock(mScriptManager);
	lua_State* L = lock.GetLuaState();

	return ScriptUtils::ToVector2(L, idx, false);
}

Point ScriptResult::ToPoint(int32 idx) const
{
	if (mNumResults == 0)
		return Point::ZERO;

	LuaStateScopedLock lock(mScriptManager);
	lua_State* L = lock.GetLuaState();

	return ScriptUtils::ToPoint(L, idx, false);
}

Size ScriptResult::ToSize(int32 idx) const
{
	if (mNumResults == 0)
		return Size::ZERO;

	LuaStateScopedLock lock(mScriptManager);
	lua_State* L = lock.GetLuaState();

	return ScriptUtils::ToPoint(L, idx, false);
}
