#pragma once
#include "../typedefs.h"

namespace core
{
	struct Vector3
	{
		union
		{
			struct
			{
				float32 x;
				float32 y;
				float32 z;
			};

			float32 _array[3];
		};

		Vector3();
		Vector3(float32 _x, float32 _y, float32 _z);
		Vector3(const Vector3& v);
		Vector3(const float32* arr);

		Vector3 GetNormalized() const;
		Vector3 CrossProduct(const Vector3& vec) const;
		float32 Length() const;
		float32 SquaredLength() const;
		bool IsZero() const;
		bool IsNotZero() const;
		float32 DotProduct(const Vector3& vec) const;
		float32 GetAngleDegree(const Vector3& vec) const;
		float32 GetAngleRadians(const Vector3& vec) const;
		void Normalize();
		void Normalize(float32 distance);
		void Negate();
		Vector3 Perp() const;

		inline Vector3 operator + (const Vector3& rhs) const {
			return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
		}

		inline Vector3 operator - (const Vector3& rhs) const {
			return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
		}

		inline Vector3 operator - () const {
			return Vector3(-x, -y, -z);
		}

		inline Vector3 operator * (const float32 rhs) const {
			return Vector3(x * rhs, y * rhs, z * rhs);
		}

		inline Vector3 operator * (const Vector3& rhs) const {
			return Vector3(x * rhs.x, y * rhs.y, z * rhs.z);
		}

		inline Vector3 operator / (float32 rhs) const {
			assert(rhs != 0.0f && "You are not allowed to divide by 0");
			rhs = 1.0f / rhs;
			return Vector3(x * rhs, y * rhs, z * rhs);
		}

		inline Vector3& operator += (const Vector3& rhs) {
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}

		inline Vector3& operator -= (const Vector3& rhs) {
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			return *this;
		}

		inline Vector3& operator *= (const float32 rhs) {
			x *= rhs;
			y *= rhs;
			z *= rhs;
			return *this;
		}

		inline Vector3& operator *= (const Vector3& rhs) {
			x *= rhs.x;
			y *= rhs.y;
			z *= rhs.z;
			return *this;
		}

		inline Vector3& operator /= (float32 scalar) {
			if(scalar != 0.0f) {
				scalar = 1.0f / scalar;
				x *= scalar;
				y *= scalar;
				z *= scalar;
			}
			return *this;
		}

		inline Vector3& operator = (const Vector3& rhs) {
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
			return *this;
		}

		inline bool operator == (const Vector3& rhs) const {
			return FLOAT_EQUALS(x, rhs.x) && FLOAT_EQUALS(y, rhs.y) && FLOAT_EQUALS(z, rhs.z);
		}

		inline bool operator != (const Vector3& rhs) const {
			return FLOAT_NEQUALS(x, rhs.x) || FLOAT_NEQUALS(y, rhs.y) || FLOAT_NEQUALS(z, rhs.z);
		}

		inline Vector3 operator - () {
			return Vector3(-x, -y, -z);
		}

		inline float32 operator[](uint32 index) const {
			return _array[index];
		}

		//
		// Parse the supplied string and return a Vector3 instance based on it
		static Vector3 Parse(const std::string& s);

		//
		// Create a normalized vector based on the supplied position coordinates
		//
		// @param x
		// @param y
		// @param z
		static Vector3 GetNormalized(float32 x, float32 y, float32 z);

		static const Vector3 ZERO;
		static const Vector3 ONE;
		static const Vector3 UNIT_X;
		static const Vector3 UNIT_Y;
		static const Vector3 UNIT_Z;
		static const Vector3 NEGATIVE_UNIT_X;
		static const Vector3 NEGATIVE_UNIT_Y;
		static const Vector3 NEGATIVE_UNIT_Z;
	};
}
