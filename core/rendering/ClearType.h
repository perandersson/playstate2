#pragma once
#include "../typedefs.h"

namespace core
{
	class ClearType
	{
	public:
		enum Enum {
			COLOR = BIT(1),
			DEPTH = BIT(2),
			BOTH = COLOR | DEPTH
		};
	};
}
