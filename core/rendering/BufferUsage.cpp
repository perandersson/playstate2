#include "../Memory.h"
#include "BufferUsage.h"
#include "../StringUtils.h"
#include "../logging/Logger.h"
using namespace core;

BufferUsage::Enum BufferUsage::Parse(const std::string& s, BufferUsage::Enum defaultValue) {
	std::hash_map<std::string, int32> enums = GetValues();
	std::hash_map<std::string, int32>::iterator it = enums.find(StringUtils::ToUpperCase(s));
	if (it == enums.end()) {
		Logger::Warn("Could not find BufferUsage '%s'. Using default instead", s.c_str());
		return defaultValue;
	}
	else {
		return (BufferUsage::Enum)it->second;
	}
}

std::hash_map<std::string, int32> BufferUsage::GetValues()
{
	std::hash_map<std::string, int32> enums;
	ENUM_STRING(enums, BufferUsage, STATIC);
	ENUM_STRING(enums, BufferUsage, DYNAMIC);
	return enums;
}
