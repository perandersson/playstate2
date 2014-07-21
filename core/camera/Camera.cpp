#include "../Memory.h"
#include "Camera.h"
#include "CameraUtils.h"
#include <string>
#include <cmath>
using namespace core;

Camera::Camera()
: ScriptObject(), mNearPlane(0), mFarPlane(0)
{
}

Camera::Camera(const Camera& camera)
: ScriptObject(),
mFrustum(camera.mFrustum), mViewMatrix(camera.mViewMatrix), mProjectionMatrix(camera.mProjectionMatrix),
mPosition(camera.mPosition), mCenter(camera.mCenter), mUp(camera.mUp), mNearPlane(0), mFarPlane(0)
{

}

Camera::~Camera()
{
}

void Camera::SetPerspective(float32 near, float32 far, float32 fov, float32 ratio)
{
	CameraUtils::GetPerspective(mProjectionMatrix, near, far, fov, ratio);
	mFrustum.SetPerspective(near, far, fov, ratio);

	mNearPlane = near;
	mFarPlane = far;
}

void Camera::SetOrtho2D(float32 left, float32 right, float32 bottom, float32 top)
{
	CameraUtils::GetOrtho2D(mProjectionMatrix, left, right, bottom, top);
}

void Camera::Move(const Vector3& direction)
{
	LookAt(mPosition + direction, mCenter + direction, mUp);
}

void Camera::LookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
{
	CameraUtils::GetLookAt(mViewMatrix, eye, center, up);
	mPosition = eye;
	mCenter = center;
	mUp = up;
	mViewDirection = (center - eye).GetNormalized();

	mFrustum.LookAt(eye, mViewDirection, mUp);
}
