#include "../Memory.h"
#include "Projector.h"
#include "CameraUtils.h"
using namespace core;

Projector::Projector()
: mNearClipDistance(0), mFarClipDistance(0)
{
}

Projector::Projector(const Projector& rhs)
: mFrustum(rhs.mFrustum), mViewMatrix(rhs.mViewMatrix), mProjectionMatrix(rhs.mProjectionMatrix),
mNearClipDistance(rhs.mNearClipDistance), mFarClipDistance(rhs.mFarClipDistance)
{

}

Projector::~Projector()
{

}

void Projector::SetPerspective(float32 near, float32 far, float32 fov, float32 ratio)
{
	CameraUtils::GetPerspective(mProjectionMatrix, near, far, fov, ratio);
	mFrustum.SetPerspective(near, far, fov, ratio);

	mNearClipDistance = near;
	mFarClipDistance = far;
}

void Projector::SetOrtho2D(float32 left, float32 right, float32 bottom, float32 top)
{
	CameraUtils::GetOrtho2D(mProjectionMatrix, left, right, bottom, top);

	mNearClipDistance = -1.0f;
	mFarClipDistance = 1.0f;
}

void Projector::LookAt(const Vector3& eye, const Vector3& direction, const Vector3& up)
{
	CameraUtils::GetLookInDirection(mViewMatrix, eye, direction, up);
	mFrustum.LookAt(eye, direction, up);
}
