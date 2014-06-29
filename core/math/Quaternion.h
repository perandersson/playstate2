#pragma once
#include "Vector3.h"

namespace core
{
	struct Quaternion
	{
		union
		{
			struct
			{
				float32 x;
				float32 y;
				float32 z;
				float32 w;
			};
			struct
			{
				Vector3 pos;
				float32 w;
			};
			float32 _array[4];
		};

		Quaternion() : x(0), y(0), z(0), w(0) {}
		Quaternion(const Quaternion& q) : x(q.x), y(q.y), z(q.z), w(q.w) {}
		
		// Operators
		Vector3 operator*(const Vector3& rhs) const;

		inline Quaternion& operator=(const Quaternion& rhs) {
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
			w = rhs.w;
			return *this;
		}
	};
}
