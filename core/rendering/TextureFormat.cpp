#include "../Memory.h"
#include "TextureFormat.h"
#include "../StringUtils.h"
#include "../logging/Logger.h"
using namespace core;

TextureFormat::Enum TextureFormat::Parse(const std::string& s, TextureFormat::Enum defaultValue) {
	std::hash_map<std::string, int32> enums = GetValues();
	std::hash_map<std::string, int32>::iterator it = enums.find(StringUtils::ToUpperCase(s));
	if (it == enums.end()) {
		Logger::Warn("Could not find TextureFormat '%s'. Using default instead", s.c_str());
		return defaultValue;
	}
	else {
		return (TextureFormat::Enum)it->second;
	}
}

std::hash_map<std::string, int32> TextureFormat::GetValues()
{
	std::hash_map<std::string, int32> enums;
	ENUM_STRING(enums, TextureFormat, R);
	ENUM_STRING(enums, TextureFormat, R16F);
	ENUM_STRING(enums, TextureFormat, R32F);
	ENUM_STRING(enums, TextureFormat, RGB);
	ENUM_STRING(enums, TextureFormat, RGB8);
	ENUM_STRING(enums, TextureFormat, RGB12);
	ENUM_STRING(enums, TextureFormat, RGB16);
	ENUM_STRING(enums, TextureFormat, RGB32);
	ENUM_STRING(enums, TextureFormat, RGBA);
	ENUM_STRING(enums, TextureFormat, RGBA8);
	ENUM_STRING(enums, TextureFormat, RGBA12);
	ENUM_STRING(enums, TextureFormat, RGBA16);
	ENUM_STRING(enums, TextureFormat, RGB10_A2);
	ENUM_STRING(enums, TextureFormat, RGBA16F);
	ENUM_STRING(enums, TextureFormat, RGBA32F);
	ENUM_STRING(enums, TextureFormat, BGR);
	ENUM_STRING(enums, TextureFormat, BGRA);
	ENUM_STRING(enums, TextureFormat, DEPTH24);
	ENUM_STRING(enums, TextureFormat, DEPTH32F);
	ENUM_STRING(enums, TextureFormat, DEPTH24_STENCIL8);
	ENUM_STRING(enums, TextureFormat, DEPTH32F_STENCIL8);
	return enums;
}
