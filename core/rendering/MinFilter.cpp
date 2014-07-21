#include "../Memory.h"
#include "MinFilter.h"
#include "../StringUtils.h"
#include "../logging/Logger.h"
using namespace core;

MinFilter::Enum MinFilter::Parse(const std::string& s, MinFilter::Enum defaultValue) {
	std::hash_map<std::string, int32> enums = GetValues();
	std::hash_map<std::string, int32>::iterator it = enums.find(StringUtils::ToUpperCase(s));
	if (it == enums.end()) {
		Logger::Warn("Could not find MinFilter '%s'. Using default instead", s.c_str());
		return defaultValue;
	}
	else {
		return (MinFilter::Enum)it->second;
	}
}

std::hash_map<std::string, int32> MinFilter::GetValues()
{
	std::hash_map<std::string, int32> enums;
	ENUM_STRING(enums, MinFilter, NEAREST);
	ENUM_STRING(enums, MinFilter, LINEAR);
	ENUM_STRING(enums, MinFilter, NEAREST_MIPMAP_NEAREST);
	ENUM_STRING(enums, MinFilter, LINEAR_MIPMAP_NEAREST);
	ENUM_STRING(enums, MinFilter, NEAREST_MIPMAP_LINEAR);
	ENUM_STRING(enums, MinFilter, LINEAR_MIPMAP_LINEAR);
	return enums;
}
