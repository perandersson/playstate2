#pragma once

namespace core
{
	class CollisionResult
	{
	public:
		enum Enum {
			OUTSIDE,
			INTERSECT,
			CONTAINED
		};
	};
}
