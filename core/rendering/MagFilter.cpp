#include "../Memory.h"
#include "MagFilter.h"
#include "../StringUtils.h"
#include "../logging/Logger.h"
using namespace core;

MagFilter::Enum MagFilter::Parse(const std::string& s, MagFilter::Enum defaultValue) {
	std::hash_map<std::string, int32> enums = GetValues();
	std::hash_map<std::string, int32>::iterator it = enums.find(StringUtils::ToUpperCase(s));
	if (it == enums.end()) {
		Logger::Warn("Could not find MagFilter '%s'. Using default instead", s.c_str());
		return defaultValue;
	}
	else {
		return (MagFilter::Enum)it->second;
	}
}

std::hash_map<std::string, int32> MagFilter::GetValues()
{
	std::hash_map<std::string, int32> enums;
	ENUM_STRING(enums, MagFilter, NEAREST);
	ENUM_STRING(enums, MagFilter, LINEAR);
	return enums;
}
