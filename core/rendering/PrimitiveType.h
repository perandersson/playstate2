#pragma once
#include "../typedefs.h"
#include "GLEWMX.h"

namespace core
{
	class PrimitiveType
	{
	public:
		enum Enum {
			POINT = 0,
			TRIANGLE,
			LINE_LOOP,

			SIZE
		};

		static uint32 GetElementCount(Enum type);
	};
}
