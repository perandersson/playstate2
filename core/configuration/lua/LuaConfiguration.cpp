#include "../../Memory.h"
#include "LuaConfiguration.h"
#include "../../script/IScriptManager.h"
using namespace core;

LuaConfiguration::LuaConfiguration(IScriptManager* scriptManager, const std::string& configFile)
: mScriptManager(scriptManager)
{
	mTable = mScriptManager->EvaluateFile(configFile).ToTable();
}

LuaConfiguration::~LuaConfiguration()
{

}

int32 LuaConfiguration::ToInt32(const char* key, int32 defaultVal) const
{
	return mTable.ToInt32(key, defaultVal);
}

int32 LuaConfiguration::ToInt32(const std::string& key, int32 defaultVal) const
{
	return mTable.ToInt32(key, defaultVal);
}

float32 LuaConfiguration::ToFloat32(const char* key, float32 defaultVal) const
{
	return mTable.ToFloat32(key, defaultVal);
}

float32 LuaConfiguration::ToFloat32(const std::string& key, float32 defaultVal) const
{
	return mTable.ToFloat32(key, defaultVal);
}

bool LuaConfiguration::ToBool(const char* key, bool defaultVal) const
{
	return mTable.ToBool(key, defaultVal);
}

bool LuaConfiguration::ToBool(const std::string& key, bool defaultVal) const
{
	return mTable.ToBool(key, defaultVal);
}

std::string LuaConfiguration::ToString(const char* key, const char* defaultVal) const
{
	return mTable.ToString(key, defaultVal);
}

std::string LuaConfiguration::ToString(const std::string& key, const char* defaultVal) const
{
	return mTable.ToString(key, defaultVal);
}

Color LuaConfiguration::ToColor(const char* key, const Color& defaultColor) const
{
	return mTable.ToColor(key, defaultColor);
}

Color LuaConfiguration::ToColor(const std::string& key, const Color& defaultColor) const
{
	return mTable.ToColor(key, defaultColor);
}

Vector2 LuaConfiguration::ToVector2(const char* key, const Vector2& defaultVector) const
{
	return mTable.ToVector2(key, defaultVector);
}

Vector2 LuaConfiguration::ToVector2(const std::string& key, const Vector2& defaultVector) const
{
	return mTable.ToVector2(key, defaultVector);
}

Point LuaConfiguration::ToPoint(const char* key, const Point& defaultVal) const
{
	return mTable.ToPoint(key, defaultVal);
}

Point LuaConfiguration::ToPoint(const std::string& key, const Point& defaultVal) const
{
	return mTable.ToPoint(key, defaultVal);
}

Size LuaConfiguration::ToSize(const char* key, const Size& defaultVal) const
{
	return mTable.ToSize(key, defaultVal);
}

Size LuaConfiguration::ToSize(const std::string& key, const Size& defaultVal) const
{
	return mTable.ToSize(key, defaultVal);
}
