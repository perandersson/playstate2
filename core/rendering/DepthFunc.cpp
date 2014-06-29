#include "../Memory.h"
#include "DepthFunc.h"
#include "../StringUtils.h"
#include "../logging/Logger.h"
using namespace core;

DepthFunc::Enum DepthFunc::Parse(const std::string& s, DepthFunc::Enum defaultValue) {
	std::hash_map<std::string, int32> enums = GetValues();
	std::hash_map<std::string, int32>::iterator it = enums.find(StringUtils::ToUpperCase(s));
	if (it == enums.end()) {
		Logger::Warn("Could not find DepthFunc '%s'. Using default instead", s.c_str());
		return defaultValue;
	}
	else {
		return (DepthFunc::Enum)it->second;
	}
}

std::hash_map<std::string, int32> DepthFunc::GetValues()
{
	std::hash_map<std::string, int32> enums;
	ENUM_STRING(enums, DepthFunc, NEVER);
	ENUM_STRING(enums, DepthFunc, LESS);
	ENUM_STRING(enums, DepthFunc, EQUAL);
	ENUM_STRING(enums, DepthFunc, LEQUAL);
	ENUM_STRING(enums, DepthFunc, GREATER);
	ENUM_STRING(enums, DepthFunc, NOTEQUAL);
	ENUM_STRING(enums, DepthFunc, GEQUAL);
	ENUM_STRING(enums, DepthFunc, ALWAYS);
	return enums;
}
