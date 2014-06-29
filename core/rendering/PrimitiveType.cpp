#include "../Memory.h"
#include "PrimitiveType.h"
using namespace core;

uint32 PrimitiveType::GetElementCount(PrimitiveType::Enum type)
{
	switch (type) {
	case POINT:
		return 1;
	case TRIANGLE:
		return 3;
	case LINE_LOOP:
		return 1;
	}

	return 0;
}
