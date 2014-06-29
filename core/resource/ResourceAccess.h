#pragma once
#include "../typedefs.h"

namespace core
{
	class ResourceAccess
	{
	public:
		enum Enum {
			NONE = 0,
			LOADING = BIT(1),
			UNLOADING = BIT(2),
			ALL = LOADING | UNLOADING
		};
	};
}
