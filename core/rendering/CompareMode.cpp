#include "../Memory.h"
#include "CompareMode.h"
#include "../StringUtils.h"
#include "../logging/Logger.h"
using namespace core;

CompareMode::Enum CompareMode::Parse(const std::string& s, CompareMode::Enum defaultValue) {
	std::hash_map<std::string, int32> enums = GetValues();
	std::hash_map<std::string, int32>::iterator it = enums.find(StringUtils::ToUpperCase(s));
	if (it == enums.end()) {
		Logger::Warn("Could not find CompareMode '%s'. Using default instead", s.c_str());
		return defaultValue;
	}
	else {
		return (CompareMode::Enum)it->second;
	}
}

std::hash_map<std::string, int32> CompareMode::GetValues()
{
	std::hash_map<std::string, int32> enums;
	ENUM_STRING(enums, CompareMode, COMPARE_R_TO_TEXTURE);
	ENUM_STRING(enums, CompareMode, NONE);
	return enums;
}
