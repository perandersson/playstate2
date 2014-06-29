#pragma once
#include "IConfiguration.h"

namespace core
{
	class Configuration
	{
	public:
		static int32 ToInt32(const char* key, int32 defaultVal = 0);
		static int32 ToInt32(const std::string& key, int32 defaultVal = 0);
		static float32 ToFloat32(const char* key, float32 defaultVal = 0.0f);
		static float32 ToFloat32(const std::string& key, float32 defaultVal = 0.0f);
		static bool ToBool(const char* key, bool defaultVal = false);
		static bool ToBool(const std::string& key, bool defaultVal = false);
		static std::string ToString(const char* key, const char* defaultVal = "");
		static std::string ToString(const std::string& key, const char* defaultVal = "");
		static Color ToColor(const char* key, const Color& defaultColor);
		static Color ToColor(const std::string& key, const Color& defaultColor);
		static Vector2 ToVector2(const char* key, const Vector2& defaultVector);
		static Vector2 ToVector2(const std::string& key, const Vector2& defaultVector);
		static Point ToPoint(const char* key, const Point& defaultVal);
		static Point ToPoint(const std::string& key, const Point& defaultVal);
		static Size ToSize(const char* key, const Size& defaultVal);
		static Size ToSize(const std::string& key, const Size& defaultVal);
	};
}
