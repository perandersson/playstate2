#include "../Memory.h"
#include "Plane.h"
using namespace core;

Plane::Plane()
{
}

Plane::Plane(const Plane& plane) 
: normal(plane.normal), point(plane.point)
{
}

Plane::Plane(const Vector3& v1, const Vector3& v2, const Vector3& v3)
{
	const Vector3 aux1 = (v1 - v2).GetNormalized();
	const Vector3 aux2 = (v3 - v2).GetNormalized();

	normal = aux2.CrossProduct(aux1);
	normal.Normalize();

	point = v2;
}

Plane::~Plane()
{
}

float32 Plane::GetDistance(const Vector3& pt) const
{
	const Vector3 delta = pt - point;
	float32 d = normal.DotProduct(delta);
	return d;
}
