#include "../Memory.h"
#include "Vector3.h"
#include <cmath>
using namespace core;

const Vector3 Vector3::ZERO = Vector3(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::ONE = Vector3(1.0f, 1.0f, 1.0f);
const Vector3 Vector3::UNIT_X = Vector3(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::UNIT_Y = Vector3(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::UNIT_Z = Vector3(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::NEGATIVE_UNIT_X = Vector3(-1.0f, 0.0f, 0.0f);
const Vector3 Vector3::NEGATIVE_UNIT_Y = Vector3(0.0f, -1.0f, 0.0f);
const Vector3 Vector3::NEGATIVE_UNIT_Z = Vector3(0.0f, 0.0f, -1.0f);

Vector3::Vector3() : x(0.0f), y(0.0f), z(0.0f)
{
}

Vector3::Vector3(float32 _x, float32 _y, float32 _z) : x(_x), y(_y), z(_z)
{
}

Vector3::Vector3(const Vector3& vector3) : x(vector3.x), y(vector3.y), z(vector3.z)
{
}

Vector3::Vector3(const float32* vector3) : x(vector3[0]), y(vector3[1]), z(vector3[2])
{
}

Vector3 Vector3::GetNormalized() const
{
	float32 len = Length();
	len = (len != 0 ? len : 1.0f);
	float32 lengthMul = 1.0f / len;

	return Vector3(x * lengthMul, y * lengthMul, z * lengthMul);
}

Vector3 Vector3::CrossProduct(const Vector3& vec) const
{
	return Vector3((y * vec.z - z * vec.y),
		(z * vec.x - x * vec.z),
		(x * vec.y - y * vec.x));
}

float32 Vector3::Length() const
{
	return sqrt(x * x + y * y + z * z);
}

float32 Vector3::SquaredLength() const
{
	return (x * x + y * y + z * z);
}

float32 Vector3::DotProduct(const Vector3& vec) const
{
	return ((vec.x * x) + (vec.y * y) + (vec.z * z));
}

float32 Vector3::GetAngleDegree(const Vector3& vec) const
{
	return acosf(((vec.x * x) + (vec.y * y) + (vec.z * z)) / (vec.Length() * Length())) * 57.29577951f;
}

float32 Vector3::GetAngleRadians(const Vector3& vec) const
{
	return acosf(((vec.x * x) + (vec.y * y) + (vec.z * z)) / (vec.Length() * Length()));
}

void Vector3::Normalize()
{
	float32 len = Length();
	len = (len != 0.0f ? len : 1.0f);

	float32 lengthMul = 1.0f / len;
	x *= lengthMul;
	y *= lengthMul;
	z *= lengthMul;
}

void Vector3::Normalize(float32 distance)
{
	float32 factor = 1.0f / distance;
	x *= factor;
	y *= factor;
	z *= factor;
}

void Vector3::Negate()
{
	x = -x;
	y = -y;
	z = -z;
}

Vector3 Vector3::Perp() const
{
	float32 min = fabs(x);
	Vector3 axis(1, 0, 0);

	const float32 ymin = fabs(y);
	if (ymin < min) {
		min = ymin;
		axis = Vector3(0, 1, 0);
	}

	const float32 zmin = fabs(z);
	if (zmin < min) {
		min = zmin;
		axis = Vector3(0, 0, 1);
	}

	return CrossProduct(axis);
}

bool Vector3::IsZero() const
{
	return abs(x) <= FLT_EPSILON &&
		abs(y) <= FLT_EPSILON &&
		abs(z) <= FLT_EPSILON;
}

bool Vector3::IsNotZero() const
{
	return abs(x) > FLT_EPSILON ||
		abs(y) > FLT_EPSILON ||
		abs(z) > FLT_EPSILON;
}

Vector3 Vector3::Parse(const std::string& s)
{
	if (s.empty())
		return Vector3::ZERO;

	Vector3 vec;
	sscanf(s.c_str(), "%f %f %f", &vec.x, &vec.y, &vec.z);
	return vec;
}

Vector3 Vector3::GetNormalized(float32 x, float32 y, float32 z)
{
	Vector3 v(x, y, z);
	return v.GetNormalized();
}
