#pragma once
#include "../typedefs.h"
#include "Point.h"

namespace core
{
	struct Rect
	{
		union 
		{
			struct 
			{
				int32 x;
				int32 y;
				int32 width;
				int32 height;
			};
			struct
			{
				Point position;
				Size size;
			};
			int32 _array[4];
		};

		//
		// 
		Rect();
		Rect(const Rect& rect);
		Rect(const Point& _position, const Size& _size);
		Rect(int32 _x, int32 _y, int32 _width, int32 _height);

		inline bool operator == (const Rect& rhs) const {
			return x == rhs.x && y == rhs.y && width == rhs.width && height == rhs.height;
		}

		inline bool operator != (const Rect& rhs) const {
			return x != rhs.x || y != rhs.y || width != rhs.width || height != rhs.height;
		}

		inline Rect& operator = (const Rect& rhs) {
			x = rhs.x;
			y = rhs.y;
			width = rhs.width;
			height = rhs.height;
			return *this;
		}

		inline Rect operator + (const Rect& rhs) const {
			return Rect(x + rhs.x, y + rhs.y, width + rhs.width, height + rhs.height);
		}
		
		inline Rect operator - (const Rect& rhs) const {
			return Rect(x - rhs.x, y - rhs.y, width - rhs.width, height - rhs.height);
		}

		//
		// Return a translated version of this rectangle instance (i.e. moved)
		//
		// @param dir The direction we want to move the new rectangle towards.
		// @return The new rectangle instance
		inline Rect GetTranslated(const Point& dir) const {
			return Rect(x + dir.x, y + dir.y, width, height);
		}

		//
		// Checks if the supplied point is inside this rectangle instance
		// 
		// @param pt 
		//			The point we want to check
		// @return TRUE if the supplied point is inside this rect
		inline bool IsPointInside(const Point& pt) const {
			if (pt.x >= (x) && pt.x <= (x + width)) {
				if (pt.y >= (y) && pt.y <= (y + height)) {
					return true;
				}
			}
			return false;
		}

		static Rect ZERO;
	};
}
