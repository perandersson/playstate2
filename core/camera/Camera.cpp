#include "../Memory.h"
#include "Camera.h"
#include <string>
#include <cmath>
using namespace core;

Camera::Camera()
: ScriptObject(), mNearPlane(0), mFarPlane(0)
{
}

Camera::Camera(const Camera& camera)
: ScriptObject(),
mViewFrustum(camera.mViewFrustum), mViewMatrix(camera.mViewMatrix), mProjectionMatrix(camera.mProjectionMatrix),
mPosition(camera.mPosition), mCenter(camera.mCenter), mUp(camera.mUp), mNearPlane(0), mFarPlane(0)
{

}

Camera::~Camera()
{
}

void Camera::SetPerspective(float32 near, float32 far, float32 fov, float32 ratio)
{
	mProjectionMatrix = GetPerspective(near, far, fov, ratio);
	mViewFrustum.SetPerspective(near, far, fov, ratio);

	mNearPlane = near;
	mFarPlane = far;
}

void Camera::SetOrtho2D(float32 left, float32 right, float32 bottom, float32 top)
{
	mProjectionMatrix = GetOrtho2D(left, right, bottom, top);
}

void Camera::Move(const Vector3& direction)
{
	LookAt(mPosition + direction, mCenter + direction, mUp);
}

void Camera::LookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
{
	mPosition = eye;
	mCenter = center;
	mUp = up;

	// http://www.opengl.org/wiki/GluLookAt_code

	const Vector3 forward = (center - eye).GetNormalized();
	mLookingAtDirection = forward;

	const Vector3 side = (forward.CrossProduct(up)).GetNormalized();
	const Vector3 newUp = side.CrossProduct(forward).GetNormalized();

	mViewMatrix = Matrix4x4(
		side.x, side.y, side.z, 0.0f,
		newUp.x, newUp.y, newUp.z, 0.0f,
		-forward.x, -forward.y, -forward.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	mViewMatrix.Translate(eye * -1.0f);
	mViewFrustum.LookAt(eye, center, up);
}

Matrix4x4 Camera::GetOrtho2D(float32 left, float32 right, float32 bottom, float32 top)
{
	// http://solarianprogrammer.com/2013/05/22/opengl-101-matrices-projection-view-model/

	const float32 near = -1;
	const float32 far = 1;

	const float32 tx = -((right + left) / (right - left));
	const float32 ty = -((top + bottom) / (top - bottom));
	const float32 tz = -((far + near) / (far - near));

	Matrix4x4 projection;
	projection._11 = 2.0f / (right - left);
	projection._14 = tx;
	projection._22 = 2.0f / (top - bottom);
	projection._24 = ty;
	projection._33 = -2.0f / (far - near);
	projection._34 = tz;
	projection._44 = 1.0f;
	return projection;
}

Matrix4x4 Camera::GetPerspective(float32 near, float32 far, float32 fov, float32 ratio)
{
	// http://solarianprogrammer.com/2013/05/22/opengl-101-matrices-projection-view-model/

	const float32 top = near * tanf(fov * (float32)(M_PI / 360.0));
	const float32 bottom = -top;
	const float32 right = top * ratio;
	const float32 left = -right;

	Matrix4x4 matrix = Matrix4x4::ZERO;
	matrix._11 = (2.0f * near) / (right - left);
	matrix._13 = (right + left) / (right - left);
	matrix._22 = (2.0f * near) / (top - bottom);
	matrix._23 = (top + bottom) / (top - bottom);
	matrix._33 = -((far + near) / (far - near));
	matrix._34 = -((2.0f * far * near) / (far - near));
	matrix._43 = -1.0f;

	return matrix;
}

Matrix4x4 Camera::GetLookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
{
	assert_not_implemented();
	return Matrix4x4::IDENTITY;
}
