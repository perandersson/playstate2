#include "../Memory.h"
#include "FrontFace.h"
#include "../StringUtils.h"
#include "../logging/Logger.h"
using namespace core;

FrontFace::Enum FrontFace::Parse(const std::string& s, FrontFace::Enum defaultValue) {
	std::hash_map<std::string, int32> enums = GetValues();
	std::hash_map<std::string, int32>::iterator it = enums.find(StringUtils::ToUpperCase(s));
	if (it == enums.end()) {
		Logger::Warn("Could not find FrontFace '%s'. Using default instead", s.c_str());
		return defaultValue;
	}
	else {
		return (FrontFace::Enum)it->second;
	}
}

std::hash_map<std::string, int32> FrontFace::GetValues()
{
	std::hash_map<std::string, int32> enums;
	ENUM_STRING(enums, FrontFace, CW);
	ENUM_STRING(enums, FrontFace, CCW);
	return enums;
}
