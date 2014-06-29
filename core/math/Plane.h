#pragma once
#include "Vector3.h"

namespace core
{
	//
	// http://en.wikipedia.org/wiki/Plane_(geometry)
	// http://www.lighthouse3d.com/tutorials/maths/plane/
	// http://www.lighthouse3d.com/tutorials/maths/
	//
	struct Plane
	{
		Vector3 normal;
		Vector3 point;

		Plane();
		Plane(const Plane& plane);
		Plane(const Vector3& v1, const Vector3& v2, const Vector3& v3);
		~Plane();

		//
		// http://www.lighthouse3d.com/tutorials/maths/plane/
		// http://mathworld.wolfram.com/Point-PlaneDistance.html
		// @remark Although not entirely true, this method returns the distance between the internal point on this plane
		//		with the supplied point. In realtity there isn't simply a "point" on the plane. This method is only used
		//		together with the frustum culling engine.
		// @return The distance between the point in the center of the plane and the supplied point.
		float32 GetDistance(const Vector3& pt) const;

		inline Plane& operator = (const Plane& rhs) {
			normal = rhs.normal;
			point = rhs.point;
			return *this;
		}
	};
}
