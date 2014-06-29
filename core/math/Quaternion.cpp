#include "../Memory.h"
#include "Quaternion.h"
using namespace core;

Vector3 Quaternion::operator * (const Vector3& rhs) const
{
	//http://molecularmusings.wordpress.com/2013/05/24/a-faster-quaternion-vector-multiplication/
	//t = 2 * cross(q.xyz, v)
	//v' = v + q.w * t + cross(q.xyz, t)

	Vector3 t = pos.CrossProduct(rhs) * 2;
	return rhs + (t * w) + pos.CrossProduct(t);
}
