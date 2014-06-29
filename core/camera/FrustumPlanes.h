#pragma once

namespace core
{
	class FrustumPlanes
	{
	public:
		enum Enum
		{
			TOP = 0,
			BOTTOM,
			LEFT,
			RIGHT,
			NEARP,
			FARP,

			NUM_PLANES
		};
	};
}
