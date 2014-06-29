#include "../Memory.h"
#include "CullFace.h"
#include "../StringUtils.h"
#include "../logging/Logger.h"
using namespace core;

CullFace::Enum CullFace::Parse(const std::string& s, CullFace::Enum defaultValue) {
	std::hash_map<std::string, int32> enums = GetValues();
	std::hash_map<std::string, int32>::iterator it = enums.find(StringUtils::ToUpperCase(s));
	if (it == enums.end()) {
		Logger::Warn("Could not find CullFace '%s'. Using default instead", s.c_str());
		return defaultValue;
	}
	else {
		return (CullFace::Enum)it->second;
	}
}

std::hash_map<std::string, int32> CullFace::GetValues()
{
	std::hash_map<std::string, int32> enums;
	ENUM_STRING(enums, CullFace, DISABLED);
	ENUM_STRING(enums, CullFace, CW);
	ENUM_STRING(enums, CullFace, CCW);
	return enums;
}
