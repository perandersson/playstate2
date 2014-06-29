#pragma once
#include "../typedefs.h"

namespace core
{
	struct Vector2
	{
		union
		{
			struct
			{
				float32 x;
				float32 y;
			};

			struct
			{
				float32 s;
				float32 t;
			};

			float32 _array[2];
		};

		Vector2();
		Vector2(float32 _x, float32 _y);
		Vector2(const Vector2 &vector2);
		Vector2 GetNormalized() const;
		float32 Length() const;
		float32 SquaredLength() const;
		float32 DotProduct(const Vector2& vec) const;
		float32 GetAngleDegree(const Vector2& vec) const;
		float32 GetAngleRadians(const Vector2& vec) const;
		void Normalize();
		void Negate();

		inline Vector2 operator + (const Vector2& rhs) const {
			return Vector2(x + rhs.x, y + rhs.y);
		}
		
		inline Vector2 operator - (const Vector2& rhs) const {
			return Vector2(x - rhs.x, y - rhs.y);
		}

		inline Vector2 operator * (const float32 rhs) const {
			return Vector2(x * rhs, y * rhs);
		}
		
		inline Vector2 operator * (const Vector2& rhs) const {
			return Vector2(x * rhs.x, y * rhs.y);
		}

		inline Vector2 operator / (const float32 rhs) const {
			assert(rhs != 0 && "You are not allowed to devide by zero");
			const float32 divd = 1.0f / rhs;
			return Vector2(x * divd, y * divd);
		}

		inline Vector2& operator += (const Vector2& rhs) {
			x += rhs.x;
			y += rhs.y;
			return *this;
		}

		inline Vector2& operator -= (const Vector2& rhs) {
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}

		inline Vector2& operator *= (const float32 rhs) {
			x *= rhs;
			y *= rhs;
			return *this;
		}

		inline Vector2& operator /= (const float32 rhs) {
			assert(rhs != 0 && "You are not allowed to devide by zero");
			const float32 divd = 1.0f / rhs;
			x *= divd;
			y *= divd;
			return *this;
		}

		inline Vector2& operator = (const Vector2& rhs) {
			x = rhs.x;
			y = rhs.y;
			return *this;
		}

		inline bool operator == (const Vector2& rhs) const {
			return FLOAT_EQUALS(x, rhs.x) && FLOAT_EQUALS(y, rhs.y);
		}

		inline bool operator != (const Vector2& rhs) const {
			return FLOAT_NEQUALS(x, rhs.x) || FLOAT_NEQUALS(y, rhs.y);
		}

		inline Vector2 operator-() {
			return Vector2(-x, -y);
		}

		static const Vector2 ZERO;
		static const Vector2 UNIT_X; 
		static const Vector2 UNIT_Y;
		static const Vector2 NEGATIVE_UNIT_X;
		static const Vector2 NEGATIVE_UNIT_Y;
	};
}
