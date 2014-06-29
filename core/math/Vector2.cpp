#include "../Memory.h"
#include "Vector2.h"
#include <cmath>
using namespace core;

const Vector2 Vector2::ZERO = Vector2(0.0f, 0.0f);
const Vector2 Vector2::UNIT_X = Vector2(1.0f, 0.0f);
const Vector2 Vector2::UNIT_Y = Vector2(0.0f, 1.0f);
const Vector2 Vector2::NEGATIVE_UNIT_X = Vector2(-1.0f, 0.0f);
const Vector2 Vector2::NEGATIVE_UNIT_Y = Vector2(0.0f, -1.0f);

Vector2::Vector2() : x(0.0f), y(0.0f)
{
}

Vector2::Vector2(float32 _x, float32 _y) : x(_x), y(_y)
{
}

Vector2::Vector2(const Vector2& v) : x(v.x), y(v.y)
{
}

Vector2 Vector2::GetNormalized() const
{
	Vector2 normalizedVector;

	float32 len = Length();
	len = (len != 0 ? len : 1.0f);

	float32 lengthMul = 1.0f / len;
	normalizedVector.x = x * lengthMul;
	normalizedVector.y = y * lengthMul;

	return normalizedVector;
}

float32 Vector2::Length() const
{
	return sqrt((x*x) + (y*y));
}

float32 Vector2::SquaredLength() const
{
	return ((x * x) + (y * y));
}

float32 Vector2::DotProduct(const Vector2& vec) const
{
	return ((vec.x * x) + (vec.y * y));
}

float32 Vector2::GetAngleDegree(const Vector2& vec) const
{
	return acosf(((vec.x * x) + (vec.y * y)) / (vec.Length() * Length())) * 57.29577951f;
}

float32 Vector2::GetAngleRadians(const Vector2& vec) const
{
	return acosf(((vec.x * x) + (vec.y * y)) / (vec.Length() * Length()));
}

void Vector2::Normalize()
{
	float32 len = Length();
	len = (len != 0 ? len : 1.0f);

	float32 lengthMul = 1.0f / len;
	x *= lengthMul;
	y *= lengthMul;
}

void Vector2::Negate()
{
	x = -x;
	y = -y;
}
