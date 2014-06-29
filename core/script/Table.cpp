#include "../Memory.h"
#include "Table.h"
#include "IScriptManager.h"
#include "../StringUtils.h"
#include "ScriptUtils.h"
using namespace core;

Table::Table()
: mScriptManager(nullptr), mScriptRef(0)
{
}

Table::Table(IScriptManager* scriptManager, uint32 scriptRef)
: mScriptManager(scriptManager), mScriptRef(scriptRef)
{
}

Table::Table(const Table& rhs)
: mScriptManager(rhs.mScriptManager), mScriptRef(0)
{
	LuaStateScopedLock lock(mScriptManager);
	lua_State* L = lock.GetLuaState();
	lua_rawgeti(L, LUA_REGISTRYINDEX, rhs.mScriptRef);
	mScriptRef = luaL_ref(L, LUA_REGISTRYINDEX);
}

Table::~Table()
{
	if (mScriptRef != 0 && mScriptManager != nullptr) {
		LuaStateScopedLock lock(mScriptManager);
		lua_State* L = lock.GetLuaState();
		luaL_unref(L, LUA_REGISTRYINDEX, mScriptRef);
		mScriptRef = 0;
	}
}

int32 Table::ToInt32(const char* key, int32 defaultVal) const
{
	return ToInt32(std::string(key), defaultVal);
}

int32 Table::ToInt32(const std::string& key, int32 defaultVal) const
{
	LuaStateScopedLock lock(mScriptManager);
	lua_State* L = lock.GetLuaState();

	std::vector<std::string> keys = StringUtils::Split(key, '>');
	std::vector<std::string>::size_type size = keys.size();
	for (uint32 i = 0; i < size; ++i) {
		if (FindKey(keys[i], L)) {
			int32 result = (int32)lua_tonumber(L, -1);
			lua_pop(L, 1);
			return result;
		}
	}

	return defaultVal;
}

float32 Table::ToFloat32(const char* key, float32 defaultVal) const
{
	return ToFloat32(std::string(key), defaultVal);
}

float32 Table::ToFloat32(const std::string& key, float32 defaultVal) const
{
	LuaStateScopedLock lock(mScriptManager);
	lua_State* L = lock.GetLuaState();

	std::vector<std::string> keys = StringUtils::Split(key, '>');
	std::vector<std::string>::size_type size = keys.size();
	for (uint32 i = 0; i < size; ++i) {
		if (FindKey(keys[i], L)) {
			float32 result = (float32)lua_tonumber(L, -1);
			lua_pop(L, 1);
			return result;
		}
	}

	return defaultVal;
}

bool Table::ToBool(const char* key, bool defaultVal) const
{
	return ToBool(std::string(key), defaultVal);
}

bool Table::ToBool(const std::string& key, bool defaultVal) const
{
	LuaStateScopedLock lock(mScriptManager);
	lua_State* L = lock.GetLuaState();

	std::vector<std::string> keys = StringUtils::Split(key, '>');
	std::vector<std::string>::size_type size = keys.size();
	for (uint32 i = 0; i < size; ++i) {
		if (FindKey(keys[i], L)) {
			bool result = lua_toboolean(L, -1) == 1;
			lua_pop(L, 1);
			return result;
		}
	}

	return defaultVal;
}

std::string Table::ToString(const char* key, const char* defaultVal) const
{
	return ToString(std::string(key), defaultVal);
}

std::string Table::ToString(const std::string& key, const char* defaultVal) const
{
	LuaStateScopedLock lock(mScriptManager);
	lua_State* L = lock.GetLuaState();

	std::vector<std::string> keys = StringUtils::Split(key, '>');
	std::vector<std::string>::size_type size = keys.size();
	for (uint32 i = 0; i < size; ++i) {
		if (FindKey(keys[i], L)) {
			std::string value = lua_tostring(L, -1);
			lua_pop(L, 1);
			return value;
		}
	}

	return defaultVal;
}

Color Table::ToColor(const char* key, const Color& defaultColor) const
{
	return ToColor(std::string(key), defaultColor);
}

Color Table::ToColor(const std::string& key, const Color& defaultColor) const
{
	LuaStateScopedLock lock(mScriptManager);
	lua_State* L = lock.GetLuaState();

	std::vector<std::string> keys = StringUtils::Split(key, '>');
	std::vector<std::string>::size_type size = keys.size();
	for (uint32 i = 0; i < size; ++i) {
		if (FindKey(keys[i], L)) {
			return ScriptUtils::ToColor(L);
		}
	}

	return defaultColor;
}

Vector2 Table::ToVector2(const char* key, const Vector2& defaultVector) const
{
	return ToVector2(std::string(key), defaultVector);
}

Vector2 Table::ToVector2(const std::string& key, const Vector2& defaultVector) const
{
	LuaStateScopedLock lock(mScriptManager);
	lua_State* L = lock.GetLuaState();

	std::vector<std::string> keys = StringUtils::Split(key, '>');
	std::vector<std::string>::size_type size = keys.size();
	for (uint32 i = 0; i < size; ++i) {
		if (FindKey(keys[i], L)) {
			return ScriptUtils::ToVector2(L);
		}
	}

	return defaultVector;
}

Point Table::ToPoint(const char* key, const Point& defaultVal) const
{
	return ToPoint(std::string(key), defaultVal);
}

Point Table::ToPoint(const std::string& key, const Point& defaultVal) const
{
	LuaStateScopedLock lock(mScriptManager);
	lua_State* L = lock.GetLuaState();

	std::vector<std::string> keys = StringUtils::Split(key, '>');
	std::vector<std::string>::size_type size = keys.size();
	for (uint32 i = 0; i < size; ++i) {
		if (FindKey(keys[i], L)) {
			return ScriptUtils::ToPoint(L);
		}
	}

	return defaultVal;
}

Size Table::ToSize(const char* key, const Size& defaultVal) const
{
	return ToSize(std::string(key), defaultVal);
}

Size Table::ToSize(const std::string& key, const Size& defaultVal) const
{
	LuaStateScopedLock lock(mScriptManager);
	lua_State* L = lock.GetLuaState();

	std::vector<std::string> keys = StringUtils::Split(key, '>');
	std::vector<std::string>::size_type size = keys.size();
	for (uint32 i = 0; i < size; ++i) {
		if (FindKey(keys[i], L)) {
			return (Size)ScriptUtils::ToPoint(L);
		}
	}

	return defaultVal;
}

ResourceData* Table::FindResourceData(const std::string& key, ResourceData* defaultObject) const
{
	LuaStateScopedLock lock(mScriptManager);
	lua_State* L = lock.GetLuaState();

	std::vector<std::string> keys = StringUtils::Split(key, '>');
	std::vector<std::string>::size_type size = keys.size();
	for (uint32 i = 0; i < size; ++i) {
		if (FindKey(keys[i], L)) {
			return ScriptUtils::ToResourceData(L);
		}
	}

	return defaultObject;
}

ScriptObject* Table::FindScriptObject(const std::string& key, ScriptObject* defaultObject) const
{
	LuaStateScopedLock lock(mScriptManager);
	lua_State* L = lock.GetLuaState();

	std::vector<std::string> keys = StringUtils::Split(key, '>');
	std::vector<std::string>::size_type size = keys.size();
	for (uint32 i = 0; i < size; ++i) {
		if (FindKey(keys[i], L)) {
			return ScriptUtils::ToScriptObject(L);
		}
	}
	return defaultObject;
}

bool Table::FindKey(const std::string& key, lua_State* L) const
{
	lua_rawgeti(L, LUA_REGISTRYINDEX, mScriptRef);
	bool istable = lua_istable(L, -1);
	std::vector<std::string> parts = StringUtils::Split(key, '.');
	std::vector<std::string>::size_type size = parts.size();
	for (uint32 i = 0; i < size; ++i) {
		const std::string& part = parts[i];
		lua_pushstring(L, part.c_str());
		lua_gettable(L, -2);
		if (lua_isnoneornil(L, -1)) {
			lua_pop(L, 2);
			return false;
		}
		lua_remove(L, -2);
	}

	return true;
}

Table& Table::operator = (const Table& rhs)
{
	mScriptManager = rhs.mScriptManager;

	LuaStateScopedLock lock(mScriptManager);
	lua_State* L = lock.GetLuaState();

	lua_rawgeti(L, LUA_REGISTRYINDEX, rhs.mScriptRef);
	uint32 newref = luaL_ref(L, LUA_REGISTRYINDEX);
	
	// If this collection already has a reference to a lua table, then release that reference first
	if (mScriptRef != 0) {
		luaL_unref(L, LUA_REGISTRYINDEX, mScriptRef);
		mScriptRef = 0;
	}

	mScriptRef = newref;
	return *this;
}
