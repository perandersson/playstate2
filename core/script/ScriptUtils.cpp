#include "../Memory.h"
#include "ScriptUtils.h"
#include "ClassLoader.h"
#include "../logging/Logger.h"
#include "ScriptObject.h"
using namespace core;

std::string ScriptUtils::PackageToFile(const std::string& package)
{
	std::string path = std::string("/") + package;
	std::string::size_type size = path.size();
	for (std::string::size_type i = 1; i < size; ++i) {
		if (path[i] == '.') {
			path[i] = '/';
		}
	}
	path += ".lua";
	return path;
}

float32 ScriptUtils::ToFloat32(lua_State* L, int idx, bool removeFromStack)
{
	assert_not_null(L);
	if (!lua_isnumber(L, idx)) {
		if (removeFromStack)
			lua_remove(L, idx);
		return 0.0f;
	}

	float32 value = (float32)lua_tonumber(L, idx);
	if (removeFromStack)
		lua_remove(L, idx);

	return value;
}

int ScriptUtils::PushFloat32(lua_State* L, float32 value)
{
	lua_pushnumber(L, value);
	return 1;
}

int32 ScriptUtils::ToInt32(lua_State* L, int idx, bool removeFromStack)
{
	assert_not_null(L);
	if (!lua_isnumber(L, idx)) {
		if (removeFromStack)
			lua_remove(L, idx);
		return 0;
	}

	int32 value = (int32)lua_tointeger(L, idx);
	if (removeFromStack)
		lua_remove(L, idx);

	return value;
}

int ScriptUtils::PushInt32(lua_State* L, int32 value)
{
	lua_pushinteger(L, value);
	return 1;
}

uint32 ScriptUtils::ToUInt32(lua_State* L, int idx, bool removeFromStack)
{
	assert_not_null(L);
	if (!lua_isnumber(L, idx)) {
		if (removeFromStack)
			lua_remove(L, idx);
		return 0;
	}

	uint32 value = (uint32)lua_tointeger(L, idx);
	if (removeFromStack)
		lua_remove(L, idx);

	return value;
}

int ScriptUtils::PushUInt32(lua_State* L, uint32 value)
{
	assert_not_null(L);
	lua_pushinteger(L, value);
	return 1;
}

typemask ScriptUtils::ToTypeMask(lua_State* L, int idx, bool removeFromStack)
{
	assert_not_null(L);
	if (!lua_isnumber(L, idx)) {
		if (removeFromStack)
			lua_remove(L, idx);
		return 0;
	}

	typemask value = (typemask)lua_tointeger(L, idx);
	if (removeFromStack)
		lua_remove(L, idx);

	return value;
}

int ScriptUtils::PushTypeMask(lua_State* L, typemask value)
{
	assert_not_null(L);
	lua_pushinteger(L, (lua_Integer)value);
	return 1;
}

void* ScriptUtils::ToPointer(lua_State* L, int idx, bool removeFromStack)
{
	assert_not_null(L);
	if (!lua_isuserdata(L, idx)) {
		if (removeFromStack)
			lua_remove(L, idx);
		return nullptr;
	}
	
	void* value = lua_touserdata(L, idx);
	if (removeFromStack)
		lua_remove(L, idx);

	return value;
}

int ScriptUtils::PushPointer(lua_State* L, const void* ptr)
{
	assert_not_null(L);
	lua_pushlightuserdata(L, (void*)ptr);
	return 1;
}

ResourceData* ScriptUtils::ToResourceData(lua_State* L, int idx, bool removeFromStack)
{
	assert_not_null(L);

	ResourceData* result = NULL;
	if (lua_isuserdata(L, idx)) {
		ResourceData** container = (ResourceData**)lua_touserdata(L, idx);
		result = *container;
	}
	if (removeFromStack)
		lua_remove(L, idx);

	return result;
}

int ScriptUtils::PushResourceData(lua_State* L, ResourceData* value)
{
	assert_not_null(L);
	assert_not_null(value);

	ResourceData** container = (ResourceData**)lua_newuserdata(L, sizeof(ResourceData**));
	*container = value;
	return 1;
}

int ScriptUtils::PushString(lua_State* L, const std::string& value)
{
	assert_not_null(L);
	lua_pushstring(L, value.c_str());
	return 1;
}

bool ScriptUtils::ToBool(lua_State* L, int idx, bool removeFromStack)
{
	assert_not_null(L);

	bool result = false;
	if (lua_isboolean(L, idx))
		result = lua_toboolean(L, idx) == 1;
	if (removeFromStack)
		lua_remove(L, idx);

	return result;
}

int ScriptUtils::PushBool(lua_State* L, bool value)
{
	assert_not_null(L);
	lua_pushboolean(L, value ? 1 : 0);
	return 1;
}

Vector2 ScriptUtils::ToVector2(lua_State* L, int idx, bool removeFromStack)
{
	assert_not_null(L);
	assert(idx < 0);

	// Make sure that the value at the supplied index is a table with the size = 2
	if (!lua_istable(L, idx) || lua_rawlen(L, idx) != 2) {
		if (removeFromStack)
			lua_remove(L, idx);
		return Vector2::ZERO;
	}

	Vector2 result;
	lua_pushnil(L);
	
	lua_next(L, idx - 1);
	result.x = ToFloat32(L);

	lua_next(L, idx - 1);
	result.y = ToFloat32(L);

	lua_next(L, idx - 1);
	if (removeFromStack)
		lua_remove(L, idx);

	return result;
}

uint32 ScriptUtils::PushVector2(lua_State* L, const Vector2& vec)
{
	assert_not_null(L);

	// local tbl = { 1 = x, 2 = y }

	lua_newtable(L);

	lua_pushnumber(L, 1);
	lua_pushnumber(L, vec.x);
	lua_rawset(L, -3); // -3 = table index

	lua_pushnumber(L, 2);
	lua_pushnumber(L, vec.y);
	lua_rawset(L, -3); // -3 = table index

	return 1;
}

Vector3 ScriptUtils::ToVector3(lua_State* L, int idx, bool removeFromStack)
{
	assert_not_null(L);
	assert(idx < 0);

	// Make sure that the value at the supplied index is a table with the size = 2
	if (!lua_istable(L, idx) || lua_rawlen(L, idx) != 3) {
		if (removeFromStack)
			lua_remove(L, idx);
		return Vector3::ZERO;
	}

	Vector3 result;
	lua_pushnil(L);

	lua_next(L, idx - 1);
	result.x = ToFloat32(L);

	lua_next(L, idx - 1);
	result.y = ToFloat32(L);

	lua_next(L, idx - 1);
	result.z = ToFloat32(L);
	
	lua_next(L, idx - 1);
	if (removeFromStack)
		lua_remove(L, idx);

	return result;
}

int ScriptUtils::PushVector3(lua_State* L, const Vector3& vec)
{
	assert_not_null(L);

	// local tbl = { 1 = x, 2 = y, 3 = z }

	lua_newtable(L);

	lua_pushnumber(L, 1);
	lua_pushnumber(L, vec.x);
	lua_rawset(L, -3); // -3 = table index

	lua_pushnumber(L, 2);
	lua_pushnumber(L, vec.y);
	lua_rawset(L, -3); // -3 = table index

	lua_pushnumber(L, 3);
	lua_pushnumber(L, vec.z);
	lua_rawset(L, -3); // -3 = table index

	return 1;
}

Color ScriptUtils::ToColor(lua_State* L, int idx, bool removeFromStack)
{
	assert_not_null(L);
	assert(idx < 0);

	if (lua_isstring(L, idx)) {
		std::string hex = ToString(L, idx, removeFromStack);
		return Color::HexToRGB(hex.c_str());
	}

	if (!lua_istable(L, idx)) {
		if (removeFromStack)
			lua_remove(L, idx);
		return Color::NOTHING;
	}

	size_t numElements = lua_rawlen(L, idx);
	Color color;
	if (numElements == 4) {
		lua_pushnil(L);

		lua_next(L, idx - 1);
		color.r = ToFloat32(L);

		lua_next(L, idx - 1);
		color.g = ToFloat32(L);

		lua_next(L, idx - 1);
		color.b = ToFloat32(L);

		lua_next(L, idx - 1);
		color.a = ToFloat32(L);
	}
	else if (numElements == 3) {
		lua_pushnil(L);

		lua_next(L, idx - 1);
		color.r = ToFloat32(L);

		lua_next(L, idx - 1);
		color.g = ToFloat32(L);

		lua_next(L, idx - 1);
		color.b = ToFloat32(L);

		// Assume that alpha is 1.0f
		color.a = 1.0f;
	}
	else if (numElements == 1) {
		lua_pushnil(L);

		lua_next(L, idx - 1);
		color.r = color.g = color.b = color.a = ToFloat32(L);
	}

	lua_next(L, idx - 1);
	if (removeFromStack)
		lua_remove(L, idx);

	return color;
}

int ScriptUtils::PushColor(lua_State* L, const Color& color)
{
	assert_not_null(L);

	// local tbl = { 1 = r, 2 = g, 3 = b, 4 = a }

	lua_newtable(L);

	lua_pushnumber(L, 1);
	lua_pushnumber(L, color.r);
	lua_rawset(L, -3); // -3 = table index

	lua_pushnumber(L, 2);
	lua_pushnumber(L, color.g);
	lua_rawset(L, -3); // -3 = table index

	lua_pushnumber(L, 3);
	lua_pushnumber(L, color.b);
	lua_rawset(L, -3); // -3 = table index

	lua_pushnumber(L, 4);
	lua_pushnumber(L, color.a);
	lua_rawset(L, -3); // -3 = table index

	return 1;
}

Point ScriptUtils::ToPoint(lua_State* L, int idx, bool removeFromStack)
{
	assert_not_null(L);
	assert(idx < 0);

	if (!lua_istable(L, idx) || lua_rawlen(L, idx) != 2) {
		if (removeFromStack)
			lua_remove(L, idx);
		return Point::ZERO;
	}

	Point value;
	lua_pushnil(L);

	lua_next(L, idx - 1);
	value.x = ToInt32(L);

	lua_next(L, idx - 1);
	value.y = ToInt32(L);

	lua_next(L, idx - 1);
	if (removeFromStack)
		lua_remove(L, idx);

	return value;
}

int ScriptUtils::PushPoint(lua_State* L, const Point& pt)
{
	assert_not_null(L);

	// local tbl = { 1 = x, 2 = y }

	lua_newtable(L);

	lua_pushnumber(L, 1);
	lua_pushinteger(L, pt.x);
	lua_rawset(L, -3); // -3 = table index

	lua_pushnumber(L, 2);
	lua_pushinteger(L, pt.y);
	lua_rawset(L, -3); // -3 = table index

	return 1;

}

Rect ScriptUtils::ToRect(lua_State* L, int idx, bool removeFromStack)
{
	assert_not_null(L);
	assert(idx < 0);

	if (!lua_istable(L, idx) || lua_rawlen(L, idx) != 4) {
		if (removeFromStack)
			lua_remove(L, idx);
		return Rect::ZERO;
	}

	Rect value;
	lua_pushnil(L);

	lua_next(L, idx - 1);
	value.x = ToInt32(L);

	lua_next(L, idx - 1);
	value.y = ToInt32(L);

	lua_next(L, idx - 1);
	value.width = ToInt32(L);

	lua_next(L, idx - 1);
	value.height = ToInt32(L);

	lua_next(L, idx - 1);
	if (removeFromStack)
		lua_remove(L, idx);

	return value;
}

ScriptObject* ScriptUtils::ToScriptObject(lua_State* L, int idx, bool removeFromStack)
{
	assert_not_null(L);
	assert(idx < 0);

	if (!lua_istable(L, idx)) {
		if (removeFromStack)
			lua_remove(L, idx);
		return nullptr;
	}

	// Try to get the pointer inside the potential weak reference container
	lua_pushstring(L, ScriptObjectTypeID);
	lua_gettable(L, idx - 1);

	if (!lua_isstring(L, -1)) {
		lua_pop(L, 1); // Remove the invalid result from the stack
		if (removeFromStack)
			lua_remove(L, idx);
		return nullptr;
	}

	const IScriptType& type = ClassLoader::GetType(lua_tostring(L, -1));
	lua_pop(L, 1); // Remove the type name from the stack

	// Try to get the pointer inside the potential weak reference container
	lua_pushstring(L, ScriptObjectRefID);
	lua_gettable(L, idx - 1);

	// If the object located in ScriptObjectRefID is NOT a userdata object then fail instantly
	if (!lua_isuserdata(L, -1)) {
		lua_pop(L, 1); // Remove the invalid result from the stack
		if (removeFromStack)
			lua_remove(L, idx);
		return nullptr;
	}

	void* userdata = lua_touserdata(L, -1);
	lua_pop(L, 1); // Remove the user-data from the stack

	if (removeFromStack)
		lua_remove(L, idx);

	// TODO: Add support for safer type-casting here!
	//ScriptObject* object = nullptr;
	///if (type.TryCastTo(userdata, (void*)&object)) {
		return reinterpret_cast<ScriptObject*>(userdata);
	//}
}

int ScriptUtils::PushObject(lua_State* L, ScriptObject* object)
{
	assert_not_null(L);

	if (object == nullptr) {
		lua_pushnil(L);
		return 1;
	}

	if (object->IsScriptBound()) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, object->GetScriptID());
		return 1;
	}

	if (object->RegisterObject())
		lua_rawgeti(L, LUA_REGISTRYINDEX, object->GetScriptID());
	else
		lua_pushnil(L);

	return 1;
}

void ScriptUtils::LogError(lua_State* L, const char* message)
{
	va_list arglist;
	va_start(arglist, message);
	char tmp[5096];
	vsprintf_s(tmp, 5096, message, arglist);
	va_end(arglist);

	lua_Debug ar;
	int32 result = lua_getstack(L, 1, &ar);
	if (result == 1) {
		lua_getinfo(L, "lS", &ar);
		const std::string identity = "filenamehere.lua";// ScriptSystem::Get().GetIdentity();
		Logger::Error("%s@%d: %s", identity.c_str(), ar.currentline, tmp);
	}
	else {
		const std::string identity = "filenamehere.lua";// ScriptSystem::Get().GetIdentity();
		Logger::Error("%s", tmp);

	}
}

std::string ScriptUtils::ToString(lua_State* L, int idx, bool removeFromStack)
{
	assert_not_null(L);
	if (!lua_isstring(L, idx)) {
		if (removeFromStack)
			lua_remove(L, idx);
		return std::string();
	}

	std::string value = lua_tostring(L, idx);
	if (removeFromStack)
		lua_remove(L, idx);

	return value;
}

void ScriptUtils::SetObject(lua_State* L, ScriptObject* object)
{
	assert_not_null(L);
	assert_not_null(object);
	assert(object->IsScriptBound() == false);

	const uint32 refID = luaL_ref(L, LUA_REGISTRYINDEX);
	object->GetScriptType()->RegisterObject(object, refID);
	object->RegisterObject(refID);
}
