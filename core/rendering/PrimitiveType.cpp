#include "../Memory.h"
#include "PrimitiveType.h"
using namespace core;

uint32 PrimitiveType::GetElementCount(PrimitiveType::Enum type)
{
	static uint32 counts[SIZE] = {
		1,
		3,
		3,
		1
	};

	return counts[(uint32)type];
}
