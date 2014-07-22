#include "../Memory.h"
#include "Matrix4x4.h"
#include <cmath>
#include <string>
using namespace core;

const Matrix4x4 Matrix4x4::IDENTITY(1, 0, 0, 0,
									0, 1, 0, 0,
									0, 0, 1, 0,
									0, 0, 0, 1);

const Matrix4x4 Matrix4x4::ZERO(0, 0, 0, 0,
								0, 0, 0, 0,
								0, 0, 0, 0,
								0, 0, 0, 0);

Matrix4x4 Matrix4x4::Translation(const Vector3& vec)
{
	return Matrix4x4(1, 0, 0, vec.x,
					 0, 1, 0, vec.y,
					 0, 0, 1, vec.z,
					 0, 0, 0, 1);
}


Matrix4x4 Matrix4x4::Rotation(const Vector3& rot)
{
	Matrix4x4 result;
	if(abs(rot.x) > 0.0f) {
		result = Matrix4x4::RotationX(rot.x);
	}

	if(abs(rot.y) > 0.0f) {
		result *= Matrix4x4::RotationY(rot.y);
	}
	
	if(abs(rot.z) > 0.0f) {
		result *= Matrix4x4::RotationZ(rot.z);
	}

	return result;
}

Matrix4x4 Matrix4x4::RotationX(float32 angleX)
{
	float cosa = cosf(angleX);
	float sina = sinf(angleX);

	return Matrix4x4(1, 0, 0, 0,
					 0, cosa, -sina, 0,
					 0, sina, cosa, 0,
					 0, 0, 0, 1);
}

Matrix4x4 Matrix4x4::RotationY(float32 angleY)
{
	float cosa = cosf(angleY);
	float sina = sinf(angleY);

	return Matrix4x4(cosa, 0, sina, 0,
					0, 1, 0, 0, 
					-sina, 0, cosa, 0,
					0, 0, 0, 1);
}

Matrix4x4 Matrix4x4::RotationZ(float32 angleZ)
{
	float cosa = cosf(angleZ);
	float sina = sinf(angleZ);

	return Matrix4x4(cosa, -sina, 0, 0,
					sina, cosa, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1);
}

Matrix4x4 Matrix4x4::Scale(float32 x, float32 y, float32 z)
{
	return Matrix4x4(x, 0, 0, 0,
					 0, y, 0, 0,
					 0, 0, z, 0,
					 0, 0, 0, 1);
}

Matrix4x4 Matrix4x4::Scale(const Vector3& scale)
{
	return Scale(scale.x, scale.y, scale.z);
}

Matrix4x4::Matrix4x4() 
: Matrix4x4(1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f)
{
}

Matrix4x4::Matrix4x4(const float32* ptr)
{
	memcpy(_array, ptr, sizeof(_array));
}

Matrix4x4::Matrix4x4(
	float32 m11, float32 m12, float32 m13, float32 m14,
	float32 m21, float32 m22, float32 m23, float32 m24,
	float32 m31, float32 m32, float32 m33, float32 m34,
	float32 m41, float32 m42, float32 m43, float32 m44) :
	_11(m11), _21(m21), _31(m31), _41(m41),
	_12(m12), _22(m22), _32(m32), _42(m42),
	_13(m13), _23(m23), _33(m33), _43(m43),
	_14(m14), _24(m24), _34(m34), _44(m44)
{
}

Matrix4x4::Matrix4x4(const Matrix4x4& mat)
{
	memcpy(_array, mat._array, sizeof(_array));
}

void Matrix4x4::Translate(const Vector3& vec)
{
	_array[3 * 4 + 0] += _array[0 * 4 + 0] * vec.x + _array[1 * 4 + 0] * vec.y + _array[2 * 4 + 0] * vec.z;
	_array[3 * 4 + 1] += _array[0 * 4 + 1] * vec.x + _array[1 * 4 + 1] * vec.y + _array[2 * 4 + 1] * vec.z;
	_array[3 * 4 + 2] += _array[0 * 4 + 2] * vec.x + _array[1 * 4 + 2] * vec.y + _array[2 * 4 + 2] * vec.z;
	_array[3 * 4 + 3] += _array[0 * 4 + 3] * vec.x + _array[1 * 4 + 3] * vec.y + _array[2 * 4 + 3] * vec.z;
}

Matrix4x4 Matrix4x4::operator * (const Matrix4x4& rhs) const
{
	// A*B != B*A

	const float32 *a = _array;
	const float32 *b = rhs._array;
	const float32 result[16] = {
		(a[0] * b[0] + a[4] * b[1] + a[8] * b[2] + a[12] * b[3]),
		(a[1] * b[0] + a[5] * b[1] + a[9] * b[2] + a[13] * b[3]),
		(a[2] * b[0] + a[6] * b[1] + a[10] * b[2] + a[14] * b[3]),
		(a[3] * b[0] + a[7] * b[1] + a[11] * b[2] + a[15] * b[3]),

		(a[0] * b[4] + a[4] * b[5] + a[8] * b[6] + a[12] * b[7]),
		(a[1] * b[4] + a[5] * b[5] + a[9] * b[6] + a[13] * b[7]),
		(a[2] * b[4] + a[6] * b[5] + a[10] * b[6] + a[14] * b[7]),
		(a[3] * b[4] + a[7] * b[5] + a[11] * b[6] + a[15] * b[7]),

		(a[0] * b[8] + a[4] * b[9] + a[8] * b[10] + a[12] * b[11]),
		(a[1] * b[8] + a[5] * b[9] + a[9] * b[10] + a[13] * b[11]),
		(a[2] * b[8] + a[6] * b[9] + a[10] * b[10] + a[14] * b[11]),
		(a[3] * b[8] + a[7] * b[9] + a[11] * b[10] + a[15] * b[11]),

		(a[0] * b[12] + a[4] * b[13] + a[8] * b[14] + a[12] * b[15]),
		(a[1] * b[12] + a[5] * b[13] + a[9] * b[14] + a[13] * b[15]),
		(a[2] * b[12] + a[6] * b[13] + a[10] * b[14] + a[14] * b[15]),
		(a[3] * b[12] + a[7] * b[13] + a[11] * b[14] + a[15] * b[15])
	};
	return Matrix4x4(result);
}

Vector3 Matrix4x4::operator * (const Vector3& rhs) const
{
	// assume W = 1.0
	const float x = _11 * rhs.x + _12 * rhs.y + _13 * rhs.z + _14 * 1.0f;
	const float y = _21 * rhs.x + _22 * rhs.y + _23 * rhs.z + _24 * 1.0f;
	const float z = _31 * rhs.x + _32 * rhs.y + _33 * rhs.z + _34 * 1.0f;

	return Vector3(x, y, z);
}

Matrix4x4& Matrix4x4::operator *= (const Matrix4x4& rhs)
{
	const float32 *a = _array;
	const float32 *b = rhs._array;

	const float32 result[16] = {
		(a[0] * b[0] + a[4] * b[1] + a[8] * b[2] + a[12] * b[3]),
		(a[1] * b[0] + a[5] * b[1] + a[9] * b[2] + a[13] * b[3]),
		(a[2] * b[0] + a[6] * b[1] + a[10] * b[2] + a[14] * b[3]),
		(a[3] * b[0] + a[7] * b[1] + a[11] * b[2] + a[15] * b[3]),

		(a[0] * b[4] + a[4] * b[5] + a[8] * b[6] + a[12] * b[7]),
		(a[1] * b[4] + a[5] * b[5] + a[9] * b[6] + a[13] * b[7]),
		(a[2] * b[4] + a[6] * b[5] + a[10] * b[6] + a[14] * b[7]),
		(a[3] * b[4] + a[7] * b[5] + a[11] * b[6] + a[15] * b[7]),

		(a[0] * b[8] + a[4] * b[9] + a[8] * b[10] + a[12] * b[11]),
		(a[1] * b[8] + a[5] * b[9] + a[9] * b[10] + a[13] * b[11]),
		(a[2] * b[8] + a[6] * b[9] + a[10] * b[10] + a[14] * b[11]),
		(a[3] * b[8] + a[7] * b[9] + a[11] * b[10] + a[15] * b[11]),

		(a[0] * b[12] + a[4] * b[13] + a[8] * b[14] + a[12] * b[15]),
		(a[1] * b[12] + a[5] * b[13] + a[9] * b[14] + a[13] * b[15]),
		(a[2] * b[12] + a[6] * b[13] + a[10] * b[14] + a[14] * b[15]),
		(a[3] * b[12] + a[7] * b[13] + a[11] * b[14] + a[15] * b[15])
	};

	memcpy(_array, result, sizeof(_array));
	return *this;
}


Matrix4x4& Matrix4x4::operator = (const Matrix4x4& matrix)
{
	memcpy(_array, matrix._array, sizeof(_array));
	return *this;
}

bool Matrix4x4::operator == (const Matrix4x4& matrix) const
{
	if (_array[0] == matrix._array[0] && _array[1] == matrix._array[1] &&
		_array[2] == matrix._array[2] && _array[3] == matrix._array[3] &&
		_array[4] == matrix._array[4] && _array[5] == matrix._array[5] &&
		_array[6] == matrix._array[6] && _array[7] == matrix._array[7] &&
		_array[8] == matrix._array[8] && _array[9] == matrix._array[9] &&
		_array[10] == matrix._array[10] && _array[11] == matrix._array[11] &&
		_array[12] == matrix._array[12] && _array[13] == matrix._array[13] &&
		_array[14] == matrix._array[14] && _array[15] == matrix._array[15])
		return true;
	return false;
}

bool Matrix4x4::operator != (const Matrix4x4& matrix) const
{
	if (_array[0] == matrix._array[0] && _array[1] == matrix._array[1] &&
		_array[2] == matrix._array[2] && _array[3] == matrix._array[3] &&
		_array[4] == matrix._array[4] && _array[5] == matrix._array[5] &&
		_array[6] == matrix._array[6] && _array[7] == matrix._array[7] &&
		_array[8] == matrix._array[8] && _array[9] == matrix._array[9] &&
		_array[10] == matrix._array[10] && _array[11] == matrix._array[11] &&
		_array[12] == matrix._array[12] && _array[13] == matrix._array[13] &&
		_array[14] == matrix._array[14] && _array[15] == matrix._array[15])
		return false;
	return true;
}

float32 Matrix4x4::operator[](uint32 index) const
{
	return _array[index];
}

void Matrix4x4::Invert()
{
	// http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix

	float32 temp[16];

	temp[0] = _array[5] * _array[10] * _array[15] -
			_array[5] * _array[11] * _array[14] -
			_array[9] * _array[6] * _array[15] +
			_array[9] * _array[7] * _array[14] +
			_array[13] * _array[6] * _array[11] -
			_array[13] * _array[7] * _array[10];

	temp[4] = -_array[4] * _array[10] * _array[15] +
		_array[4] * _array[11] * _array[14] +
		_array[8] * _array[6] * _array[15] -
		_array[8] * _array[7] * _array[14] -
		_array[12] * _array[6] * _array[11] +
		_array[12] * _array[7] * _array[10];

	temp[8] = _array[4] * _array[9] * _array[15] -
		_array[4] * _array[11] * _array[13] -
		_array[8] * _array[5] * _array[15] +
		_array[8] * _array[7] * _array[13] +
		_array[12] * _array[5] * _array[11] -
		_array[12] * _array[7] * _array[9];

	temp[12] = -_array[4] * _array[9] * _array[14] +
		_array[4] * _array[10] * _array[13] +
		_array[8] * _array[5] * _array[14] -
		_array[8] * _array[6] * _array[13] -
		_array[12] * _array[5] * _array[10] +
		_array[12] * _array[6] * _array[9];

	temp[1] = -_array[1] * _array[10] * _array[15] +
		_array[1] * _array[11] * _array[14] +
		_array[9] * _array[2] * _array[15] -
		_array[9] * _array[3] * _array[14] -
		_array[13] * _array[2] * _array[11] +
		_array[13] * _array[3] * _array[10];

	temp[5] = _array[0] * _array[10] * _array[15] -
		_array[0] * _array[11] * _array[14] -
		_array[8] * _array[2] * _array[15] +
		_array[8] * _array[3] * _array[14] +
		_array[12] * _array[2] * _array[11] -
		_array[12] * _array[3] * _array[10];

	temp[9] = -_array[0] * _array[9] * _array[15] +
		_array[0] * _array[11] * _array[13] +
		_array[8] * _array[1] * _array[15] -
		_array[8] * _array[3] * _array[13] -
		_array[12] * _array[1] * _array[11] +
		_array[12] * _array[3] * _array[9];

	temp[13] = _array[0] * _array[9] * _array[14] -
		_array[0] * _array[10] * _array[13] -
		_array[8] * _array[1] * _array[14] +
		_array[8] * _array[2] * _array[13] +
		_array[12] * _array[1] * _array[10] -
		_array[12] * _array[2] * _array[9];

	temp[2] = _array[1] * _array[6] * _array[15] -
		_array[1] * _array[7] * _array[14] -
		_array[5] * _array[2] * _array[15] +
		_array[5] * _array[3] * _array[14] +
		_array[13] * _array[2] * _array[7] -
		_array[13] * _array[3] * _array[6];

	temp[6] = -_array[0] * _array[6] * _array[15] +
		_array[0] * _array[7] * _array[14] +
		_array[4] * _array[2] * _array[15] -
		_array[4] * _array[3] * _array[14] -
		_array[12] * _array[2] * _array[7] +
		_array[12] * _array[3] * _array[6];

	temp[10] = _array[0] * _array[5] * _array[15] -
		_array[0] * _array[7] * _array[13] -
		_array[4] * _array[1] * _array[15] +
		_array[4] * _array[3] * _array[13] +
		_array[12] * _array[1] * _array[7] -
		_array[12] * _array[3] * _array[5];

	temp[14] = -_array[0] * _array[5] * _array[14] +
		_array[0] * _array[6] * _array[13] +
		_array[4] * _array[1] * _array[14] -
		_array[4] * _array[2] * _array[13] -
		_array[12] * _array[1] * _array[6] +
		_array[12] * _array[2] * _array[5];

	temp[3] = -_array[1] * _array[6] * _array[11] +
		_array[1] * _array[7] * _array[10] +
		_array[5] * _array[2] * _array[11] -
		_array[5] * _array[3] * _array[10] -
		_array[9] * _array[2] * _array[7] +
		_array[9] * _array[3] * _array[6];

	temp[7] = _array[0] * _array[6] * _array[11] -
		_array[0] * _array[7] * _array[10] -
		_array[4] * _array[2] * _array[11] +
		_array[4] * _array[3] * _array[10] +
		_array[8] * _array[2] * _array[7] -
		_array[8] * _array[3] * _array[6];

	temp[11] = -_array[0] * _array[5] * _array[11] +
		_array[0] * _array[7] * _array[9] +
		_array[4] * _array[1] * _array[11] -
		_array[4] * _array[3] * _array[9] -
		_array[8] * _array[1] * _array[7] +
		_array[8] * _array[3] * _array[5];

	temp[15] = _array[0] * _array[5] * _array[10] -
		_array[0] * _array[6] * _array[9] -
		_array[4] * _array[1] * _array[10] +
		_array[4] * _array[2] * _array[9] +
		_array[8] * _array[1] * _array[6] -
		_array[8] * _array[2] * _array[5];

	float32 det = _array[0] * temp[0] + _array[1] * temp[4] + _array[2] * temp[8] + _array[3] * temp[12];
	assert(FLOAT_NEQUALS(det, 0));
	det = 1.0 / det;
	for (uint32 i = 0; i < 16; i++)
		_array[i] = temp[i] * det;
}

Matrix4x4 Matrix4x4::GetInverted() const
{
	// http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix

	float32 temp[16];

	temp[0] = _array[5] * _array[10] * _array[15] -
		_array[5] * _array[11] * _array[14] -
		_array[9] * _array[6] * _array[15] +
		_array[9] * _array[7] * _array[14] +
		_array[13] * _array[6] * _array[11] -
		_array[13] * _array[7] * _array[10];

	temp[4] = -_array[4] * _array[10] * _array[15] +
		_array[4] * _array[11] * _array[14] +
		_array[8] * _array[6] * _array[15] -
		_array[8] * _array[7] * _array[14] -
		_array[12] * _array[6] * _array[11] +
		_array[12] * _array[7] * _array[10];

	temp[8] = _array[4] * _array[9] * _array[15] -
		_array[4] * _array[11] * _array[13] -
		_array[8] * _array[5] * _array[15] +
		_array[8] * _array[7] * _array[13] +
		_array[12] * _array[5] * _array[11] -
		_array[12] * _array[7] * _array[9];

	temp[12] = -_array[4] * _array[9] * _array[14] +
		_array[4] * _array[10] * _array[13] +
		_array[8] * _array[5] * _array[14] -
		_array[8] * _array[6] * _array[13] -
		_array[12] * _array[5] * _array[10] +
		_array[12] * _array[6] * _array[9];

	temp[1] = -_array[1] * _array[10] * _array[15] +
		_array[1] * _array[11] * _array[14] +
		_array[9] * _array[2] * _array[15] -
		_array[9] * _array[3] * _array[14] -
		_array[13] * _array[2] * _array[11] +
		_array[13] * _array[3] * _array[10];

	temp[5] = _array[0] * _array[10] * _array[15] -
		_array[0] * _array[11] * _array[14] -
		_array[8] * _array[2] * _array[15] +
		_array[8] * _array[3] * _array[14] +
		_array[12] * _array[2] * _array[11] -
		_array[12] * _array[3] * _array[10];

	temp[9] = -_array[0] * _array[9] * _array[15] +
		_array[0] * _array[11] * _array[13] +
		_array[8] * _array[1] * _array[15] -
		_array[8] * _array[3] * _array[13] -
		_array[12] * _array[1] * _array[11] +
		_array[12] * _array[3] * _array[9];

	temp[13] = _array[0] * _array[9] * _array[14] -
		_array[0] * _array[10] * _array[13] -
		_array[8] * _array[1] * _array[14] +
		_array[8] * _array[2] * _array[13] +
		_array[12] * _array[1] * _array[10] -
		_array[12] * _array[2] * _array[9];

	temp[2] = _array[1] * _array[6] * _array[15] -
		_array[1] * _array[7] * _array[14] -
		_array[5] * _array[2] * _array[15] +
		_array[5] * _array[3] * _array[14] +
		_array[13] * _array[2] * _array[7] -
		_array[13] * _array[3] * _array[6];

	temp[6] = -_array[0] * _array[6] * _array[15] +
		_array[0] * _array[7] * _array[14] +
		_array[4] * _array[2] * _array[15] -
		_array[4] * _array[3] * _array[14] -
		_array[12] * _array[2] * _array[7] +
		_array[12] * _array[3] * _array[6];

	temp[10] = _array[0] * _array[5] * _array[15] -
		_array[0] * _array[7] * _array[13] -
		_array[4] * _array[1] * _array[15] +
		_array[4] * _array[3] * _array[13] +
		_array[12] * _array[1] * _array[7] -
		_array[12] * _array[3] * _array[5];

	temp[14] = -_array[0] * _array[5] * _array[14] +
		_array[0] * _array[6] * _array[13] +
		_array[4] * _array[1] * _array[14] -
		_array[4] * _array[2] * _array[13] -
		_array[12] * _array[1] * _array[6] +
		_array[12] * _array[2] * _array[5];

	temp[3] = -_array[1] * _array[6] * _array[11] +
		_array[1] * _array[7] * _array[10] +
		_array[5] * _array[2] * _array[11] -
		_array[5] * _array[3] * _array[10] -
		_array[9] * _array[2] * _array[7] +
		_array[9] * _array[3] * _array[6];

	temp[7] = _array[0] * _array[6] * _array[11] -
		_array[0] * _array[7] * _array[10] -
		_array[4] * _array[2] * _array[11] +
		_array[4] * _array[3] * _array[10] +
		_array[8] * _array[2] * _array[7] -
		_array[8] * _array[3] * _array[6];

	temp[11] = -_array[0] * _array[5] * _array[11] +
		_array[0] * _array[7] * _array[9] +
		_array[4] * _array[1] * _array[11] -
		_array[4] * _array[3] * _array[9] -
		_array[8] * _array[1] * _array[7] +
		_array[8] * _array[3] * _array[5];

	temp[15] = _array[0] * _array[5] * _array[10] -
		_array[0] * _array[6] * _array[9] -
		_array[4] * _array[1] * _array[10] +
		_array[4] * _array[2] * _array[9] +
		_array[8] * _array[1] * _array[6] -
		_array[8] * _array[2] * _array[5];

	float32 det = _array[0] * temp[0] + _array[1] * temp[4] + _array[2] * temp[8] + _array[3] * temp[12];
	assert(FLOAT_NEQUALS(det, 0));
	det = 1.0 / det;
	Matrix4x4 mat;
	for (uint32 i = 0; i < 16; i++)
		mat._array[i] = temp[i] * det;
	return mat;
}

