#include "../Memory.h"
#include "CompareFunc.h"
#include "../StringUtils.h"
#include "../logging/Logger.h"
using namespace core;

CompareFunc::Enum CompareFunc::Parse(const std::string& s, CompareFunc::Enum defaultValue) {
	std::hash_map<std::string, int32> enums = GetValues();
	std::hash_map<std::string, int32>::iterator it = enums.find(StringUtils::ToUpperCase(s));
	if (it == enums.end()) {
		Logger::Warn("Could not find CompareFunc '%s'. Using default instead", s.c_str());
		return defaultValue;
	}
	else {
		return (CompareFunc::Enum)it->second;
	}
}

std::hash_map<std::string, int32> CompareFunc::GetValues()
{
	std::hash_map<std::string, int32> enums;
	ENUM_STRING(enums, CompareFunc, NEVER);
	ENUM_STRING(enums, CompareFunc, LESS);
	ENUM_STRING(enums, CompareFunc, EQUAL);
	ENUM_STRING(enums, CompareFunc, LEQUAL);
	ENUM_STRING(enums, CompareFunc, GREATER);
	ENUM_STRING(enums, CompareFunc, NOTEQUAL);
	ENUM_STRING(enums, CompareFunc, GEQUAL);
	ENUM_STRING(enums, CompareFunc, ALWAYS);
	return enums;
}
