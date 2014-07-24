#pragma once
#include "../typedefs.h"

namespace core
{
	struct Point
	{
		union
		{
			struct
			{
				int32 x;
				int32 y;
			};
			struct
			{
				int32 width;
				int32 height;
			};
			int32 _array[2];
		};

		Point();
		Point(const Point& point);
		Point(int32 _x, int32 _y);

		inline Point& operator = (const Point& rhs) {
			x = rhs.x;
			y = rhs.y;
			return *this;
		}

		inline bool operator == (const Point& pt) const {
			return x == pt.x && y == pt.y;
		}

		inline bool operator != (const Point& pt) const {
			return x != pt.x || y != pt.y;
		}

		inline bool operator >= (const Point& pt) const {
			return x >= pt.x && y >= pt.y;
		}
		
		inline bool operator <= (const Point& pt) const {
			return x <= pt.x && y <= pt.y;
		}

		inline Point operator + (const Point& rhs) const {
			return Point(x + rhs.x, y + rhs.y);
		}

		inline Point operator - (const Point& rhs) const {
			return Point(x - rhs.x, y - rhs.y);
		}

		inline Point operator / (const float32 rhs) const {
			return Point((int32)(x / rhs), (int32)(y / rhs));
		}

		inline Point operator * (const float32 rhs) const {
			return Point((int32)(x * rhs), (int32)(y * rhs));
		}

		static Point ZERO;
	};

	typedef Point Size;
}

