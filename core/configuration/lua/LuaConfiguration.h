#pragma once
#include "../IConfiguration.h"
#include "../../script/Table.h"

namespace core
{
	class LuaConfiguration : public IConfiguration
	{
	public:
		LuaConfiguration(IScriptManager* scriptManager, const std::string& configFile);
		virtual ~LuaConfiguration();

	// IConfiguration
	public:
		virtual int32 ToInt32(const char* key, int32 defaultVal = 0) const;
		virtual int32 ToInt32(const std::string& key, int32 defaultValue = 0) const;
		virtual float32 ToFloat32(const char* key, float32 defaultVal = 0.0f) const;
		virtual float32 ToFloat32(const std::string& key, float32 defaultVal = 0.0f) const;
		virtual bool ToBool(const char* key, bool defaultVal = false) const;
		virtual bool ToBool(const std::string& key, bool defaultVal = false) const;
		virtual std::string ToString(const char* key, const char* defaultVal = "") const;
		virtual std::string ToString(const std::string& key, const char* defaultVal = "") const;
		virtual Color ToColor(const char* key, const Color& defaultColor) const;
		virtual Color ToColor(const std::string& key, const Color& defaultColor) const;
		virtual Vector2 ToVector2(const char* key, const Vector2& defaultVector) const;
		virtual Vector2 ToVector2(const std::string& key, const Vector2& defaultVector) const;
		virtual Point ToPoint(const char* key, const Point& defaultVal) const;
		virtual Point ToPoint(const std::string& key, const Point& defaultVal) const;
		virtual Size ToSize(const char* key, const Size& defaultVal) const;
		virtual Size ToSize(const std::string& key, const Size& defaultVal) const;

	private:
		IScriptManager* mScriptManager;
		Table mTable;
	};
}
