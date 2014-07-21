#pragma once
#include "../typedefs.h"

namespace core
{
	class FindQueryFilter
	{
	public:
		enum Enum {
			GEOMETRY = BIT(1),
			TEXTURES = BIT(2),

			//
			// LIGHT SOURCES
			//

			POINT_LIGHTS = BIT(3),
			SPOT_LIGHTS = BIT(4),

			//
			// SHADOW
			//

			SHADOW_CASTER = BIT(5),

			DEBUG_INFO = BIT(6),
		};

		static const uint32 DEFAULT = GEOMETRY | TEXTURES;
		static const uint32 ALL_LIGHTS = POINT_LIGHTS | SPOT_LIGHTS;
	};

}
