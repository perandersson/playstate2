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
			STENCIL = BIT(3),
		};

		/* Clear the color- and depth buffers */
		static const uint32 COLOR_AND_DEPTH = COLOR | DEPTH;

		/* Clear the depth- and stencil buffers */
		static const uint32 DEPTH_AND_STENCIL = DEPTH | STENCIL;

		/* Clear all buffers */
		static const uint32 ALL = COLOR | DEPTH | STENCIL;
	};
}
