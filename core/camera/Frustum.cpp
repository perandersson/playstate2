#include "../Memory.h"
#include "Frustum.h"
#include <cmath>
using namespace core;

Frustum::Frustum()
	: mNearPlane(FLT_MIN), mFarPlane(FLT_MIN), mFov(FLT_MIN), mRatio(FLT_MIN),
	mTang(FLT_MIN), mNH(FLT_MIN), mNW(FLT_MIN), mFH(FLT_MIN), mFW(FLT_MIN)
{
}

Frustum::Frustum(const Frustum& frustum)
	: mNearPlane(frustum.mNearPlane), mFarPlane(frustum.mFarPlane), mFov(frustum.mFov), mRatio(frustum.mRatio),
	mTang(frustum.mTang), mNH(frustum.mNH), mNW(frustum.mNW), mFH(frustum.mFH), mFW(frustum.mFW)
{
	memcpy(mPlanes, frustum.mPlanes, sizeof(mPlanes));
}

Frustum::Frustum(const Vector3& eye, const Vector3& center, const Vector3& up,
	float32 nearPlane, float32 farPlane, float32 fov, float32 ratio)
	: Frustum()
{
	SetPerspective(nearPlane, farPlane, fov, ratio);
	LookAt(eye, center, up);
}

Frustum::~Frustum()
{
}

void Frustum::SetPerspective(float32 nearPlane, float32 farPlane, float32 fov, float32 ratio)
{
	mNearPlane = nearPlane;
	mFarPlane = farPlane;
	mFov = fov > 170.0f ? 170.0f : fov;
	mRatio = ratio;

	assert(mNearPlane > FLT_MIN && "Invalid near plane");
	assert(mFarPlane > FLT_MIN && "Invalid near plane");
	assert(mFov > FLT_MIN && "Invalid near plane");
	assert(mRatio > FLT_MIN && "Invalid near plane");

	// compute width and height of the near and far plane sections
	mTang = (float32)tan(ANG2RAD * (double)fov * 0.5) ;
	mNH = nearPlane * mTang;
	mNW = mNH * ratio;
	mFH = farPlane  * mTang;
	mFW = mFH * ratio;
}

void Frustum::LookAt(const Vector3& eye, const Vector3& direction, const Vector3& up)
{
	Vector3 dir, nc, fc, X, Y, Z;
	
	// compute the Z axis of camera
	// this axis points in the opposite direction from
	// the looking direction
	Z = -direction;

	// X axis of camera with given "up" vector and Z axis
	X = up.CrossProduct(Z).GetNormalized();

	// the real "up" vector is the cross product of Z and X
	Y = Z.CrossProduct(X);

	// compute the centers of the near and far planes
	nc = eye - Z * mNearPlane;
	fc = eye - Z * mFarPlane;

	// compute the 4 corners of the frustum on the near plane
	Vector3 ntl = nc + Y * mNH - X * mNW;
	Vector3 ntr = nc + Y * mNH + X * mNW;
	Vector3 nbl = nc - Y * mNH - X * mNW;
	Vector3 nbr = nc - Y * mNH + X * mNW;

	// compute the 4 corners of the frustum on the far plane
	Vector3 ftl = fc + Y * mFH - X * mFW;
	Vector3 ftr = fc + Y * mFH + X * mFW;
	Vector3 fbl = fc - Y * mFH - X * mFW;
	Vector3 fbr = fc - Y * mFH + X * mFW;

	// compute the six planes
	mPlanes[FrustumPlanes::TOP] = Plane(ntr, ntl, ftl);
	mPlanes[FrustumPlanes::BOTTOM] = Plane(nbl, nbr, fbr);
	mPlanes[FrustumPlanes::LEFT] = Plane(ntl, nbl, fbl);
	mPlanes[FrustumPlanes::RIGHT] = Plane(nbr, ntr, fbr);
	mPlanes[FrustumPlanes::NEARP] = Plane(ntl, ntr, nbr);
	mPlanes[FrustumPlanes::FARP] = Plane(ftr, ftl, fbl);
}

CollisionResult::Enum Frustum::IsColliding(const AABB& box) const
{
	// http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-testing-boxes-ii/
	// Later optimizations: http://www.cescg.org/CESCG-2002/DSykoraJJelinek/index.html

	CollisionResult::Enum result = CollisionResult::CONTAINED;
	for(int i=0; i < 6; i++) {
		const Plane& plane = mPlanes[i];
		// is the positive vertex outside?
		if (plane.GetDistance(GetPositiveVertex(box, plane.normal)) < 0)
			return CollisionResult::OUTSIDE;

		// is the negative vertex outside?
		else if (plane.GetDistance(GetNegativeVertex(box, plane.normal)) < 0)
			result = CollisionResult::INTERSECT;
	}

	return result;
}

Vector3 Frustum::GetPositiveVertex(const AABB& boundingBox, const Vector3& normal) const
{
	Vector3 p = boundingBox.minPoint;
	if (normal.x >= 0)
		p.x = boundingBox.maxPoint.x;
	if (normal.y >= 0)
		p.y = boundingBox.maxPoint.y;
	if (normal.z >= 0)
		p.z = boundingBox.maxPoint.z;

	return p;
}

Vector3 Frustum::GetNegativeVertex(const AABB& boundingBox, const Vector3& normal) const
{
	Vector3 n = boundingBox.maxPoint;
	if (normal.x >= 0)
		n.x = boundingBox.minPoint.x;
	if (normal.y >= 0)
		n.y = boundingBox.minPoint.y;
	if (normal.z >= 0)
		n.z = boundingBox.minPoint.z;

	return n;
}

