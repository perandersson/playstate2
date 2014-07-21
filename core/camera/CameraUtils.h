#pragma once
#include "../math/Matrix4x4.h"
#include "../math/Vector3.h"

namespace core
{
	class CameraUtils
	{
	public:
		/*!
			\brief Generates a perspective projection matrix

			\param near
			\param far
			\param fov
			\param ratio

			\return
		*/
		static Matrix4x4 GetPerspective(float32 near, float32 far, float32 fov, float32 ratio)
		{
			Matrix4x4 matrix;
			GetPerspective(matrix, near, far, fov, ratio);
			return matrix;
		}
		
		/*!
			\brief Generates a perspective projection matrix

			\param mat
			\param near
			\param far
			\param fov
			\param ratio
		*/
		static void GetPerspective(Matrix4x4& mat, float32 near, float32 far, float32 fov, float32 ratio)
		{
			// http://solarianprogrammer.com/2013/05/22/opengl-101-matrices-projection-view-model/

			const float32 top = near * tanf(fov * (float32)(M_PI / 360.0));
			const float32 bottom = -top;
			const float32 right = top * ratio;
			const float32 left = -right;

			mat._11 = (2.0f * near) / (right - left);
			mat._12 = 0.0f;
			mat._13 = (right + left) / (right - left);
			mat._14 = 0.0f;
			mat._21 = 0.0f;
			mat._22 = (2.0f * near) / (top - bottom);
			mat._23 = (top + bottom) / (top - bottom);
			mat._24 = 0.0f;
			mat._31 = 0.0f;
			mat._32 = 0.0f;
			mat._33 = -((far + near) / (far - near));
			mat._34 = -((2.0f * far * near) / (far - near));
			mat._41 = 0.0f;
			mat._42 = 0.0f;
			mat._43 = -1.0f;
			mat._44 = 0.0f;
		}
		
		/*!
			\brief Generates a look-at view matrix

			\param eye
			\param center
			\param up
			\return
		*/
		static Matrix4x4 GetLookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
		{
			// http://www.opengl.org/wiki/GluLookAt_code

			const Vector3 forward = (center - eye).GetNormalized();
			return GetLookInDirection(eye, forward, up);
		}
		
		/*!
			\brief Generates a look-at view matrix

			\param mat
			\param eye
			\param center
			\param up
		*/
		static void GetLookAt(Matrix4x4& mat, const Vector3& eye, const Vector3& center, const Vector3& up)
		{
			// http://www.opengl.org/wiki/GluLookAt_code

			const Vector3 forward = (center - eye).GetNormalized();
			return GetLookInDirection(mat, eye, forward, up);
		}
		
		/*!
			\brief Generates a look-at view matrix

			\param eye
			\param direction
			\param up
			\return
		*/
		static Matrix4x4 GetLookInDirection(const Vector3& eye, const Vector3& direction, const Vector3& up)
		{
			// http://www.opengl.org/wiki/GluLookAt_code
			Matrix4x4 mat;
			GetLookInDirection(mat, eye, direction, up);
			return mat;
		}
		
		/*!
			\brief Generates a look-at view matrix

			\param mat
			\param eye
			\param direction
			\param up
		*/
		static void GetLookInDirection(Matrix4x4& mat, const Vector3& eye, const Vector3& direction, const Vector3& up)
		{
			// http://www.opengl.org/wiki/GluLookAt_code

			const Vector3 u = up.GetNormalized();

			const Vector3 side = (direction.CrossProduct(u)).GetNormalized();
			const Vector3 newUp = side.CrossProduct(direction);

			mat._11 = side.x;
			mat._12 = side.y;
			mat._13 = side.z;
			mat._14 = 0.0f;

			mat._21 = newUp.x;
			mat._22 = newUp.y;
			mat._23 = newUp.z;
			mat._24 = 0.0f;

			mat._31 = -direction.x;
			mat._32 = -direction.y;
			mat._33 = -direction.z;
			mat._34 = 0.0f;

			mat._41 = 0.0f;
			mat._42 = 0.0f;
			mat._43 = 0.0f;
			mat._44 = 1.0f;

			mat.Translate(eye * -1.0f);
		}
		
		/*!
			\brief Generate a orthographic projection matrix

			\param left
			\param right
			\param bottom
			\param top

			\return
		*/
		static Matrix4x4 GetOrtho2D(float32 left, float32 right, float32 bottom, float32 top)
		{
			Matrix4x4 mat;
			GetOrtho2D(mat, left, right, bottom, top);
			return mat;
		}
		
		/*!
			\brief Generate a orthographic projection matrix

			\param mat
			\param left
			\param right
			\param bottom
			\param top
		*/
		static void GetOrtho2D(Matrix4x4& mat, float32 left, float32 right, float32 bottom, float32 top)
		{
			// http://solarianprogrammer.com/2013/05/22/opengl-101-matrices-projection-view-model/

			const float32 near = -1;
			const float32 far = 1;

			const float32 tx = -((right + left) / (right - left));
			const float32 ty = -((top + bottom) / (top - bottom));
			const float32 tz = -((far + near) / (far - near));

			mat._11 = 2.0f / (right - left);
			mat._12 = 0.0f;
			mat._13 = 0.0f;
			mat._14 = tx;
			mat._21 = 0.0f;
			mat._22 = 2.0f / (top - bottom);
			mat._23 = 0.0f;
			mat._24 = ty;
			mat._31 = 0.0f;
			mat._32 = 0.0f;
			mat._33 = -2.0f / (far - near);
			mat._34 = tz;
			mat._41 = 0.0f;
			mat._42 = 0.0f;
			mat._43 = 0.0f;
			mat._44 = 1.0f;
		}
	};
}
