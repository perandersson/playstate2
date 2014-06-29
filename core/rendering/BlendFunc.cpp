#include "../Memory.h"
#include "BlendFunc.h"
#include "../StringUtils.h"
#include "../logging/Logger.h"
using namespace core;

SrcFactor::Enum SrcFactor::Parse(const std::string& s, SrcFactor::Enum defaultValue) {
	std::hash_map<std::string, int32> enums = GetValues();
	std::hash_map<std::string, int32>::iterator it = enums.find(StringUtils::ToUpperCase(s));
	if (it == enums.end()) {
		Logger::Warn("Could not find SrcFactor '%s'. Using default instead", s.c_str());
		return defaultValue;
	}
	else {
		return (SrcFactor::Enum)it->second;
	}
}

std::hash_map<std::string, int32> SrcFactor::GetValues()
{
	std::hash_map<std::string, int32> enums;
	ENUM_STRING(enums, SrcFactor, ZERO);
	ENUM_STRING(enums, SrcFactor, ONE);
	ENUM_STRING(enums, SrcFactor, SRC_COLOR);
	ENUM_STRING(enums, SrcFactor, ONE_MINUS_SRC_COLOR);
	ENUM_STRING(enums, SrcFactor, SRC_ALPHA);
	ENUM_STRING(enums, SrcFactor, ONE_MINUS_SRC_ALPHA);
	ENUM_STRING(enums, SrcFactor, ONE_MINUS_DST_ALPHA);
	ENUM_STRING(enums, SrcFactor, DST_COLOR);
	ENUM_STRING(enums, SrcFactor, ONE_MINUS_DST_COLOR);
	ENUM_STRING(enums, SrcFactor, CONSTANT_COLOR);
	ENUM_STRING(enums, SrcFactor, ONE_MINUS_CONSTANT_COLOR);
	ENUM_STRING(enums, SrcFactor, CONSTANT_ALPHA);
	ENUM_STRING(enums, SrcFactor, ONE_MINUS_CONSTANT_ALPHA);
	return enums;
}

DestFactor::Enum DestFactor::Parse(const std::string& s, DestFactor::Enum defaultValue) {
	std::hash_map<std::string, int32> enums = GetValues();
	std::hash_map<std::string, int32>::iterator it = enums.find(StringUtils::ToUpperCase(s));
	if (it == enums.end()) {
		Logger::Warn("Could not find DestFactor '%s'. Using default instead", s.c_str());
		return defaultValue;
	}
	else {
		return (DestFactor::Enum)it->second;
	}
}

std::hash_map<std::string, int32> DestFactor::GetValues()
{
	std::hash_map<std::string, int32> enums;
	ENUM_STRING(enums, DestFactor, ZERO);
	ENUM_STRING(enums, DestFactor, ONE);
	ENUM_STRING(enums, DestFactor, SRC_COLOR);
	ENUM_STRING(enums, DestFactor, ONE_MINUS_SRC_COLOR);
	ENUM_STRING(enums, DestFactor, SRC_ALPHA);
	ENUM_STRING(enums, DestFactor, ONE_MINUS_SRC_ALPHA);
	ENUM_STRING(enums, DestFactor, ONE_MINUS_DST_ALPHA);
	ENUM_STRING(enums, DestFactor, DST_COLOR);
	ENUM_STRING(enums, DestFactor, ONE_MINUS_DST_COLOR);
	ENUM_STRING(enums, DestFactor, CONSTANT_COLOR);
	ENUM_STRING(enums, DestFactor, ONE_MINUS_CONSTANT_COLOR);
	ENUM_STRING(enums, DestFactor, CONSTANT_ALPHA);
	ENUM_STRING(enums, DestFactor, ONE_MINUS_CONSTANT_ALPHA);
	return enums;
}
