#pragma once
#include "Vector3.h"

namespace core
{
	/*!
		\brief A column-major 4x4 Matrix implementation
	*/
	struct Matrix4x4
	{
		union
		{
			struct {
				float32 
				_11, _21, _31, _41,
				_12, _22, _32, _42,
				_13, _23, _33, _43,
				_14, _24, _34, _44;
			};
			struct {
				/* Array block containing the the actual values. The first part is the column index and the second is the row index */
				float32 m[4][4];
			};

			/* Array block containing the actual values. */
			float32 _array[16];
		};

		Matrix4x4();
		Matrix4x4(const float32* ptr);
		Matrix4x4(float32 m11, float32 m12, float32 m13, float32 m14,
				  float32 m21, float32 m22, float32 m23, float32 m24,
				  float32 m31, float32 m32, float32 m33, float32 m34,
				  float32 m41, float32 m42, float32 m43, float32 m44);
		Matrix4x4(const Matrix4x4& mat);

		void Translate(const Vector3& vec);

		Matrix4x4 operator * (const Matrix4x4& rhs) const;

		/*!
			\brief Mutliplies this matrix with the supplied vector. 

			It is assumed that a vector's W component is 1.0

			\param rhs
					The value on the right-hand side of this object
			\return A 3D vector
		*/
		Vector3 operator * (const Vector3& rhs) const;

		Matrix4x4& operator *= (const Matrix4x4& rhs);

		Matrix4x4& operator = (const Matrix4x4& rhs);

		bool operator == (const Matrix4x4& rhs) const;
		bool operator != (const Matrix4x4& rhs) const;

		float32 operator[](uint32 index) const;

		static const Matrix4x4 IDENTITY;
		static const Matrix4x4 ZERO;

		static Matrix4x4 Translation(const Vector3& vec);
		static Matrix4x4 Rotation(const Vector3& rot);
		static Matrix4x4 RotationX(float32 angleX);
		static Matrix4x4 RotationY(float32 angleY);
		static Matrix4x4 RotationZ(float32 angleZ);
		static Matrix4x4 Scale(float32 x, float32 y, float32 z);
		static Matrix4x4 Scale(const Vector3& scale);
	};
}
