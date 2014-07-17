#include "../Memory.h"
#include "PolygonMode.h"
#include "../StringUtils.h"
#include "../logging/Logger.h"
using namespace core;

PolygonMode::Enum PolygonMode::Parse(const std::string& s, PolygonMode::Enum defaultValue) {
	std::hash_map<std::string, int32> enums = GetValues();
	std::hash_map<std::string, int32>::iterator it = enums.find(StringUtils::ToUpperCase(s));
	if (it == enums.end()) {
		Logger::Warn("Could not find PolygonMode '%s'. Using default instead", s.c_str());
		return defaultValue;
	}
	else {
		return (PolygonMode::Enum)it->second;
	}
}

std::hash_map<std::string, int32> PolygonMode::GetValues()
{
	std::hash_map<std::string, int32> enums;
	ENUM_STRING(enums, PolygonMode, POINT);
	ENUM_STRING(enums, PolygonMode, LINE);
	ENUM_STRING(enums, PolygonMode, FILL);
	return enums;
}
