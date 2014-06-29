#include "../Memory.h"
#include "Configuration.h"
#include "../kernel/Kernel.h"
using namespace core;

int32 Configuration::ToInt32(const char* key, int32 defaultVal)
{
	return Kernel::GetConfiguration()->ToInt32(key, defaultVal);
}

int32 Configuration::ToInt32(const std::string& key, int32 defaultVal)
{
	return Kernel::GetConfiguration()->ToInt32(key, defaultVal);
}

float32 Configuration::ToFloat32(const char* key, float32 defaultVal)
{
	return Kernel::GetConfiguration()->ToFloat32(key, defaultVal);
}

float32 Configuration::ToFloat32(const std::string& key, float32 defaultVal)
{
	return Kernel::GetConfiguration()->ToFloat32(key, defaultVal);
}

bool Configuration::ToBool(const char* key, bool defaultVal)
{
	return Kernel::GetConfiguration()->ToBool(key, defaultVal);
}

bool Configuration::ToBool(const std::string& key, bool defaultVal)
{
	return Kernel::GetConfiguration()->ToBool(key, defaultVal);
}

std::string Configuration::ToString(const char* key, const char* defaultVal)
{
	return Kernel::GetConfiguration()->ToString(key, defaultVal);
}

std::string Configuration::ToString(const std::string& key, const char* defaultVal)
{
	return Kernel::GetConfiguration()->ToString(key, defaultVal);
}

Color Configuration::ToColor(const char* key, const Color& defaultColor)
{
	return Kernel::GetConfiguration()->ToColor(key, defaultColor);
}

Color Configuration::ToColor(const std::string& key, const Color& defaultColor)
{
	return Kernel::GetConfiguration()->ToColor(key, defaultColor);
}

Vector2 Configuration::ToVector2(const char* key, const Vector2& defaultVector)
{
	return Kernel::GetConfiguration()->ToVector2(key, defaultVector);
}

Vector2 Configuration::ToVector2(const std::string& key, const Vector2& defaultVector)
{
	return Kernel::GetConfiguration()->ToVector2(key, defaultVector);
}

Point Configuration::ToPoint(const char* key, const Point& defaultVal)
{
	return Kernel::GetConfiguration()->ToPoint(key, defaultVal);
}

Point Configuration::ToPoint(const std::string& key, const Point& defaultVal)
{
	return Kernel::GetConfiguration()->ToPoint(key, defaultVal);
}

Size Configuration::ToSize(const char* key, const Size& defaultVal)
{
	return Kernel::GetConfiguration()->ToSize(key, defaultVal);
}

Size Configuration::ToSize(const std::string& key, const Size& defaultVal)
{
	return Kernel::GetConfiguration()->ToSize(key, defaultVal);
}
