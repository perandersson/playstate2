#pragma once
#include "Vector3.h"

namespace core
{
	struct Matrix4x4
	{
		union
		{
			struct {
				float32 _11, _12, _13, _14;
				float32 _21, _22, _23, _24;
				float32 _31, _32, _33, _34;
				float32 _41, _42, _43, _44;
			};
			struct {
				float32 r1[4];
				float32 r2[4];
				float32 r3[4];
				float32 r4[4];
			};
			float32 _array[16];
		};

		Matrix4x4();
		Matrix4x4(float32 m11, float32 m12, float32 m13, float32 m14,
				  float32 m21, float32 m22, float32 m23, float32 m24,
				  float32 m31, float32 m32, float32 m33, float32 m34,
				  float32 m41, float32 m42, float32 m43, float32 m44);
		Matrix4x4(const Matrix4x4& mat);

		//void Zero();
		void Translate(const Vector3& vec);
		float32 Determinant();
		void Transpose();
		void Invert();
		void AxisAndAngleToMatrix(Vector3& vector, float32 angleRadians);
		
		Matrix4x4 operator + (const Matrix4x4& rhs) const;
		Matrix4x4 operator - (const Matrix4x4& rhs) const;
		Matrix4x4 operator * (const Matrix4x4& rhs) const;
		Vector3 operator * (const Vector3& rhs) const;

		Matrix4x4& operator += (const Matrix4x4& rhs);
		Matrix4x4& operator -= (const Matrix4x4& rhs);
		Matrix4x4& operator *= (const Matrix4x4& rhs);
		Matrix4x4& operator *= (const float32 rhs);
		Matrix4x4& operator /= (const float32 rhs);

		Matrix4x4 operator * (const float32 rhs) const;
		Matrix4x4 operator / (const float32 rhs) const;

		Matrix4x4& operator = (const Matrix4x4& rhs);

		bool operator == (const Matrix4x4& rhs) const;
		bool operator != (const Matrix4x4& rhs) const;

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
