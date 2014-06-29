#include "../Memory.h"
#include "MouseButton.h"
using namespace core;

std::hash_map<std::string, int32> MouseButton::GetValues()
{
	std::hash_map<std::string, int32> enums;
	ENUM_STRING(enums, MouseButton, LEFT);
	ENUM_STRING(enums, MouseButton, MIDDLE);
	ENUM_STRING(enums, MouseButton, RIGHT);
	return enums;
}
