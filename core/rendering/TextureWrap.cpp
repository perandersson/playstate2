#include "../Memory.h"
#include "TextureWrap.h"
#include "../StringUtils.h"
#include "../logging/Logger.h"
using namespace core;

TextureWrap::Enum TextureWrap::Parse(const std::string& s, TextureWrap::Enum defaultValue) {
	std::hash_map<std::string, int32> enums = GetValues();
	std::hash_map<std::string, int32>::iterator it = enums.find(StringUtils::ToUpperCase(s));
	if (it == enums.end()) {
		Logger::Warn("Could not find TextureWrap '%s'. Using default instead", s.c_str());
		return defaultValue;
	}
	else {
		return (TextureWrap::Enum)it->second;
	}
}

GLenum TextureWrap::Parse(TextureWrap::Enum e)
{
	static const GLenum enums[TextureWrap::SIZE] = {
		GL_CLAMP_TO_EDGE,
		GL_CLAMP_TO_BORDER,
		GL_MIRRORED_REPEAT,
		GL_REPEAT
	};

	return enums[(uint32)e];
}

std::hash_map<std::string, int32> TextureWrap::GetValues()
{
	std::hash_map<std::string, int32> enums;
	ENUM_STRING(enums, TextureWrap, CLAMP_TO_EDGE);
	ENUM_STRING(enums, TextureWrap, CLAMP_TO_BORDER);
	ENUM_STRING(enums, TextureWrap, MIRRORED_REPEAT);
	ENUM_STRING(enums, TextureWrap, REPEAT);
	return enums;
}
