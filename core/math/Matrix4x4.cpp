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
	// OpenGL = Transposed versions of this
	// http://www.3dcodingtutorial.com/Basic-OpenGL-functions/Translate-and-Rotate-functions.html
	return Matrix4x4(1, 0, 0, 0,
					 0, 1, 0, 0,
					 0, 0, 1, 0,
					 vec.x, vec.y, vec.z, 1);
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
	result.Transpose();
	return result;
}

Matrix4x4 Matrix4x4::RotationX(float32 angleX)
{
	// OpenGL = Transposed versions of this
	// http://www.3dcodingtutorial.com/Basic-OpenGL-functions/Translate-and-Rotate-functions.html

	float cosa = cosf(angleX);
	float sina = sinf(angleX);

	return Matrix4x4(1.0f, 0.0f, 0.0f, 0.0f,
					 0.0f, cosa, -sina, 0.0f,
					 0.0f, sina, cosa, 0.0f,
					 0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4x4 Matrix4x4::RotationY(float32 angleY)
{
	// OpenGL = Transposed versions of this
	// http://www.3dcodingtutorial.com/Basic-OpenGL-functions/Translate-and-Rotate-functions.html

	float cosa = cosf(angleY);
	float sina = sinf(angleY);

	return Matrix4x4(cosa, 0.0f, sina, 0.0f,
					 0.0f, 1.0f, 0.0f, 0.0f,
					 -sina, 0.0f, cosa, 0.0f,
					 0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4x4 Matrix4x4::RotationZ(float32 angleZ)
{
	// OpenGL = Transposed versions of this
	// http://www.3dcodingtutorial.com/Basic-OpenGL-functions/Translate-and-Rotate-functions.html

	float cosa = cosf(angleZ);
	float sina = sinf(angleZ);

	return Matrix4x4(cosa, -sina, 0.0f, 0.0f,
					 sina, cosa, 0.0f, 0.0f,
					 0.0f, 0.0f, 1.0f, 0.0f,
					 0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4x4 Matrix4x4::Scale(float32 x, float32 y, float32 z)
{
	// http://nehe.gamedev.net/article/matrices/19004/
	return Matrix4x4(x, 0, 0, 0,
					 0, y, 0, 0,
					 0, 0, z, 0,
					 0, 0, 0, 1);
}

Matrix4x4 Matrix4x4::Scale(const Vector3& scale)
{
	return Scale(scale.x, scale.y, scale.z);
}

Matrix4x4::Matrix4x4() : _11(1), _12(0), _13(0), _14(0), 
						 _21(0), _22(1), _23(0), _24(0),
						 _31(0), _32(0), _33(1), _34(0),
						 _41(0), _42(0), _43(0), _44(1)
{
}

Matrix4x4::Matrix4x4(float32 m11, float32 m12, float32 m13, float32 m14,
					 float32 m21, float32 m22, float32 m23, float32 m24,
					 float32 m31, float32 m32, float32 m33, float32 m34,
					 float32 m41, float32 m42, float32 m43, float32 m44)
{
	_array[0] = m11;
	_array[1] = m12;
	_array[2] = m13;
	_array[3] = m14;
	_array[4] = m21;
	_array[5] = m22;
	_array[6] = m23;
	_array[7] = m24;
	_array[8] = m31;
	_array[9] = m32;
	_array[10] = m33;
	_array[11] = m34;
	_array[12] = m41;
	_array[13] = m42;
	_array[14] = m43;
	_array[15] = m44;
}

Matrix4x4::Matrix4x4(const Matrix4x4& mat)
{
	_array[0] = mat._array[0];
	_array[1] = mat._array[1];
	_array[2] = mat._array[2];
	_array[3] = mat._array[3];
	_array[4] = mat._array[4];
	_array[5] = mat._array[5];
	_array[6] = mat._array[6];
	_array[7] = mat._array[7];
	_array[8] = mat._array[8];
	_array[9] = mat._array[9];
	_array[10] = mat._array[10];
	_array[11] = mat._array[11];
	_array[12] = mat._array[12];
	_array[13] = mat._array[13];
	_array[14] = mat._array[14];
	_array[15] = mat._array[15];
}

void Matrix4x4::Translate(const Vector3& vec)
{
	_array[3 * 4 + 0] += _array[0 * 4 + 0] * vec.x + _array[1 * 4 + 0] * vec.y + _array[2 * 4 + 0] * vec.z;
	_array[3 * 4 + 1] += _array[0 * 4 + 1] * vec.x + _array[1 * 4 + 1] * vec.y + _array[2 * 4 + 1] * vec.z;
	_array[3 * 4 + 2] += _array[0 * 4 + 2] * vec.x + _array[1 * 4 + 2] * vec.y + _array[2 * 4 + 2] * vec.z;
	_array[3 * 4 + 3] += _array[0 * 4 + 3] * vec.x + _array[1 * 4 + 3] * vec.y + _array[2 * 4 + 3] * vec.z;
}

float Matrix4x4::Determinant()
{
	float32 subMatrix0[9];
	subMatrix0[0] = _array[5]; subMatrix0[1] = _array[6]; subMatrix0[2] = _array[7];
	subMatrix0[3] = _array[9]; subMatrix0[4] = _array[10]; subMatrix0[5] = _array[11];
	subMatrix0[6] = _array[13]; subMatrix0[7] = _array[14]; subMatrix0[8] = _array[15];

	float32 subMatrix1[9];
	subMatrix1[0] = _array[4]; subMatrix1[1] = _array[6]; subMatrix1[2] = _array[7];
	subMatrix1[3] = _array[8]; subMatrix1[4] = _array[10]; subMatrix1[5] = _array[11];
	subMatrix1[6] = _array[12]; subMatrix1[7] = _array[14]; subMatrix1[8] = _array[15];

	float32 subMatrix2[9];
	subMatrix2[0] = _array[4]; subMatrix2[1] = _array[5]; subMatrix2[2] = _array[7];
	subMatrix2[3] = _array[8]; subMatrix2[4] = _array[9]; subMatrix2[5] = _array[11];
	subMatrix2[6] = _array[12]; subMatrix2[7] = _array[13]; subMatrix2[8] = _array[15];

	float32 subMatrix3[9];
	subMatrix3[0] = _array[4]; subMatrix3[1] = _array[5]; subMatrix3[2] = _array[6];
	subMatrix3[3] = _array[8]; subMatrix3[4] = _array[9]; subMatrix3[5] = _array[10];
	subMatrix3[6] = _array[12]; subMatrix3[7] = _array[13]; subMatrix3[8] = _array[14];

	float32 subDeterminant0 =subMatrix0[0] * (subMatrix0[4] * subMatrix0[8] - subMatrix0[7] * subMatrix0[5]) -
								subMatrix0[1] * (subMatrix0[3] * subMatrix0[8] - subMatrix0[6] * subMatrix0[5]) +
								subMatrix0[2] * (subMatrix0[3] * subMatrix0[7] - subMatrix0[6] * subMatrix0[4]);
	float32 subDeterminant1 =subMatrix1[0] * (subMatrix1[4] * subMatrix1[8] - subMatrix1[7] * subMatrix1[5]) -
								subMatrix1[1] * (subMatrix1[3] * subMatrix1[8] - subMatrix1[6] * subMatrix1[5]) +
								subMatrix1[2] * (subMatrix1[3] * subMatrix1[7] - subMatrix1[6] * subMatrix1[4]);
	float32 subDeterminant2 =subMatrix2[0] * (subMatrix2[4] * subMatrix2[8] - subMatrix2[7] * subMatrix2[5]) -
								subMatrix2[1] * (subMatrix2[3] * subMatrix2[8] - subMatrix2[6] * subMatrix2[5]) +
								subMatrix2[2] * (subMatrix2[3] * subMatrix2[7] - subMatrix2[6] * subMatrix2[4]);
	float32 subDeterminant3 =subMatrix3[0] * (subMatrix3[4] * subMatrix3[8] - subMatrix3[7] * subMatrix3[5]) -
								subMatrix3[1] * (subMatrix3[3] * subMatrix3[8] - subMatrix3[6] * subMatrix3[5]) +
								subMatrix3[2] * (subMatrix3[3] * subMatrix3[7] - subMatrix3[6] * subMatrix3[4]);
			
	float32 determinant = (_array[0] * subDeterminant0) - (_array[1] * subDeterminant1) +
		(_array[2] * subDeterminant2) - (_array[3] * subDeterminant3);

	return determinant;
}

void Matrix4x4::Transpose()
{
	float32 tempMatrix[16];
	tempMatrix[0] = _array[0];
	tempMatrix[1] = _array[1];
	tempMatrix[2] = _array[2];
	tempMatrix[3] = _array[3];
	tempMatrix[4] = _array[4];
	tempMatrix[5] = _array[5];
	tempMatrix[6] = _array[6];
	tempMatrix[7] = _array[7];
	tempMatrix[8] = _array[8];
	tempMatrix[9] = _array[9];
	tempMatrix[10] = _array[10];
	tempMatrix[11] = _array[11];
	tempMatrix[12] = _array[12];
	tempMatrix[13] = _array[13];
	tempMatrix[14] = _array[14];
	tempMatrix[15] = _array[15];

	_array[1] = tempMatrix[4];
	_array[2] = tempMatrix[8];
	_array[3] = tempMatrix[12];
	_array[4] = tempMatrix[1];
	_array[6] = tempMatrix[9];
	_array[7] = tempMatrix[13];
	_array[8] = tempMatrix[2];
	_array[9] = tempMatrix[6];
	_array[11] = tempMatrix[14];
	_array[12] = tempMatrix[3];
	_array[13] = tempMatrix[7];
	_array[14] = tempMatrix[11];
}

void Matrix4x4::Invert()
{
	unsigned int row, column, rowMax;
	float32 tmp;
	float32 *matrixA = _array;
	Matrix4x4 identity;
	float32 *matrixB = identity._array;

	for(column = 0; column < 4; column++)
	{
		rowMax = column;
		for(row = column + 1; row < 4; row++)
		{
			if(fabs(matrixA[4 * row + column]) > fabs(matrixA[4 * rowMax + column]))
			{
				rowMax = row;
			}
		}
		if(matrixA[4 * column + rowMax] == 0.0f)
		{
			*this = Matrix4x4::IDENTITY;
			return;
		}
		for(int c = 0; c < 4; c++)
		{
			tmp = matrixA[4 * column + c];
			matrixA[4 * column + c] = matrixA[4 * rowMax + c];
			matrixA[4 * rowMax + c] = tmp;

			tmp = matrixB[4 * column + c];
			matrixB[4 * column + c] = matrixB[4 * rowMax + c];
			matrixB[4 * rowMax + c] = tmp;
		}
		tmp = matrixA[4 * column + column];
		for(int c = 0; c < 4; c++)
		{
			matrixA[4 * column + c] /= tmp;
			matrixB[4 * column + c] /= tmp;
		}
		for(row = 0; row < 4; row++)
		{
			if(row != column)
			{
				tmp = matrixA[4 * row + column];
				for(int c = 0; c < 4; c++)
				{
					matrixA[4 * row + c] -= matrixA[4 * column + c] * tmp;
					matrixB[4 * row + c] -= matrixB[4 * column + c] * tmp;
				}
			}
		}

	}

	memcpy(_array, matrixB, sizeof(_array));
}

void Matrix4x4::AxisAndAngleToMatrix(Vector3 &vector, float32 angleRadians)
{
	const float32 cosAngle = cosf(angleRadians);
	const float32 sinAngle = sinf(angleRadians);
	const float32 t = 1 - cosAngle;

	_array[0] = t * vector.x * vector.x + cosAngle;
	_array[1] = t * vector.x * vector.y - vector.z * sinAngle;
	_array[2] = t * vector.x * vector.z + vector.y * sinAngle;
	_array[3] = 0;
			
	_array[4] = t * vector.x * vector.y + vector.z * sinAngle;
	_array[5] = t * vector.y * vector.y + cosAngle;
	_array[6] = t * vector.y * vector.z - vector.x * sinAngle;
	_array[7] = 0;

	_array[8] = t * vector.x * vector.z - vector.y * sinAngle;
	_array[9] = t * vector.y * vector.z + vector.x * sinAngle;
	_array[10] = t * vector.z * vector.z + cosAngle;
	_array[11] = 0;

	_array[12] = 0;
	_array[13] = 0;
	_array[14] = 0;
	_array[15] = 1;
}

Matrix4x4 Matrix4x4::operator + (const Matrix4x4& rhs) const
{
	const float32 *m = _array;
	const float32 *n = rhs._array;

	return Matrix4x4(
		m[0]  + n[0],  m[1]  + n[1],  m[2]  + n[2],  m[3]  + n[3],
		m[4]  + n[4],  m[5]  + n[5],  m[6]  + n[6],  m[7]  + n[7], 
		m[8]  + n[8],  m[9]  + n[9],  m[10] + n[10], m[11] + n[11], 
		m[12] + n[12], m[13] + n[13], m[14] + n[14], m[15] + n[15]);
}

Matrix4x4 Matrix4x4::operator - (const Matrix4x4& rhs) const
{
	const float32 *m = _array;
	const float32 *n = rhs._array;

	return Matrix4x4(
		m[0]  - n[0],  m[1]  - n[1],  m[2]  - n[2],  m[3]  - n[3],
		m[4]  - n[4],  m[5]  - n[5],  m[6]  - n[6],  m[7]  - n[7], 
		m[8]  - n[8],  m[9]  - n[9],  m[10] - n[10], m[11] - n[11], 
		m[12] - n[12], m[13] - n[13], m[14] - n[14], m[15] - n[15]);
}

Matrix4x4 Matrix4x4::operator * (const Matrix4x4& rhs) const
{
	const float32 *n = _array;
	const float32 *m = rhs._array;

	return Matrix4x4(
		(m[0] * n[0] + m[4] * n[1] + m[8] * n[2] + m[12] * n[3]),
		(m[1] * n[0] + m[5] * n[1] + m[9] * n[2] + m[13] * n[3]),
		(m[2] * n[0] + m[6] * n[1] + m[10] * n[2] + m[14] * n[3]),
		(m[3] * n[0] + m[7] * n[1] + m[11] * n[2] + m[15] * n[3]),

		(m[0] * n[4] + m[4] * n[5] + m[8] * n[6] + m[12] * n[7]),
		(m[1] * n[4] + m[5] * n[5] + m[9] * n[6] + m[13] * n[7]),
		(m[2] * n[4] + m[6] * n[5] + m[10] * n[6] + m[14] * n[7]),
		(m[3] * n[4] + m[7] * n[5] + m[11] * n[6] + m[15] * n[7]),

		(m[0] * n[8] + m[4] * n[9] + m[8] * n[10] + m[12] * n[11]),
		(m[1] * n[8] + m[5] * n[9] + m[9] * n[10] + m[13] * n[11]),
		(m[2] * n[8] + m[6] * n[9] + m[10] * n[10] + m[14] * n[11]),
		(m[3] * n[8] + m[7] * n[9] + m[11] * n[10] + m[15] * n[11]),

		(m[0] * n[12] + m[4] * n[13] + m[8] * n[14] + m[12] * n[15]),
		(m[1] * n[12] + m[5] * n[13] + m[9] * n[14] + m[13] * n[15]),
		(m[2] * n[12] + m[6] * n[13] + m[10] * n[14] + m[14] * n[15]),
		(m[3] * n[12] + m[7] * n[13] + m[11] * n[14] + m[15] * n[15]));
}

Vector3 Matrix4x4::operator * (const Vector3& rhs) const
{
	// assume W = 1.0
	const float x = _11 * rhs.x + _12 * rhs.y + _13 * rhs.z + _14 * 1.0f;
	const float y = _21 * rhs.x + _22 * rhs.y + _23 * rhs.z + _24 * 1.0f;
	const float z = _31 * rhs.x + _32 * rhs.y + _33 * rhs.z + _34 * 1.0f;

	return Vector3(x, y, z);
}

Matrix4x4& Matrix4x4::operator += (const Matrix4x4& rhs)
{
	_array[0] += rhs._array[0];
	_array[1] += rhs._array[1];
	_array[2] += rhs._array[2];
	_array[3] += rhs._array[3];
	_array[4] += rhs._array[4];
	_array[5] += rhs._array[5];
	_array[6] += rhs._array[6];
	_array[7] += rhs._array[7];
	_array[8] += rhs._array[8];
	_array[9] += rhs._array[9];
	_array[10] += rhs._array[10];
	_array[11] += rhs._array[11];
	_array[12] += rhs._array[12];
	_array[13] += rhs._array[13];
	_array[14] += rhs._array[14];
	_array[15] += rhs._array[15];
	return *this;
}

Matrix4x4& Matrix4x4::operator -= (const Matrix4x4& rhs)
{
	_array[0] -= rhs._array[0];
	_array[1] -= rhs._array[1];
	_array[2] -= rhs._array[2];
	_array[3] -= rhs._array[3];
	_array[4] -= rhs._array[4];
	_array[5] -= rhs._array[5];
	_array[6] -= rhs._array[6];
	_array[7] -= rhs._array[7];
	_array[8] -= rhs._array[8];
	_array[9] -= rhs._array[9];
	_array[10] -= rhs._array[10];
	_array[11] -= rhs._array[11];
	_array[12] -= rhs._array[12];
	_array[13] -= rhs._array[13];
	_array[14] -= rhs._array[14];
	_array[15] -= rhs._array[15];
	return *this;
}

Matrix4x4& Matrix4x4::operator *= (const Matrix4x4& rhs)
{
	const float32 *n = _array;
	const float32 *m = rhs._array;

	Matrix4x4 tempMatrix(
	(m[0] * n[0] + m[4] * n[1] + m[8] * n[2] + m[12] * n[3]),
	(m[1] * n[0] + m[5] * n[1] + m[9] * n[2] + m[13] * n[3]),
	(m[2] * n[0] + m[6] * n[1] + m[10] * n[2] + m[14] * n[3]),
	(m[3] * n[0] + m[7] * n[1] + m[11] * n[2] + m[15] * n[3]),

	(m[0] * n[4] + m[4] * n[5] + m[8] * n[6] + m[12] * n[7]),
	(m[1] * n[4] + m[5] * n[5] + m[9] * n[6] + m[13] * n[7]),
	(m[2] * n[4] + m[6] * n[5] + m[10] * n[6] + m[14] * n[7]),
	(m[3] * n[4] + m[7] * n[5] + m[11] * n[6] + m[15] * n[7]),

	(m[0] * n[8] + m[4] * n[9] + m[8] * n[10] + m[12] * n[11]),
	(m[1] * n[8] + m[5] * n[9] + m[9] * n[10] + m[13] * n[11]),
	(m[2] * n[8] + m[6] * n[9] + m[10] * n[10] + m[14] * n[11]),
	(m[3] * n[8] + m[7] * n[9] + m[11] * n[10] + m[15] * n[11]),

	(m[0] * n[12] + m[4] * n[13] + m[8] * n[14] + m[12] * n[15]),
	(m[1] * n[12] + m[5] * n[13] + m[9] * n[14] + m[13] * n[15]),
	(m[2] * n[12] + m[6] * n[13] + m[10] * n[14] + m[14] * n[15]),
	(m[3] * n[12] + m[7] * n[13] + m[11] * n[14] + m[15] * n[15]));

	memcpy(_array, tempMatrix._array, sizeof(_array));
	return *this;
}

Matrix4x4 Matrix4x4::operator * (const float32 rhs) const
{
	return Matrix4x4(
		_array[0] * rhs, _array[1] * rhs, _array[2] * rhs, _array[3] * rhs,
		_array[4] * rhs, _array[5] * rhs, _array[6] * rhs, _array[7] * rhs,
		_array[8] * rhs, _array[9] * rhs, _array[10] * rhs, _array[11] * rhs,
		_array[12] * rhs, _array[13] * rhs, _array[14] * rhs, _array[15] * rhs);
}

Matrix4x4 Matrix4x4::operator / (const float32 rhs) const
{
	return Matrix4x4(
		_array[0] / rhs, _array[1] / rhs, _array[2] / rhs, _array[3] / rhs,
		_array[4] / rhs, _array[5] / rhs, _array[6] / rhs, _array[7] / rhs,
		_array[8] / rhs, _array[9] / rhs, _array[10] / rhs, _array[11] / rhs,
		_array[12] / rhs, _array[13] / rhs, _array[14] / rhs, _array[15] / rhs);
}

Matrix4x4& Matrix4x4::operator *= (const float32 rhs)
{
	_array[0] *= rhs;
	_array[1] *= rhs;
	_array[2] *= rhs;
	_array[3] *= rhs;
	_array[4] *= rhs;
	_array[5] *= rhs;
	_array[6] *= rhs;
	_array[7] *= rhs;
	_array[8] *= rhs;
	_array[9] *= rhs;
	_array[10] *= rhs;
	_array[11] *= rhs;
	_array[12] *= rhs;
	_array[13] *= rhs;
	_array[14] *= rhs;
	_array[15] *= rhs;
	return *this;
}

Matrix4x4& Matrix4x4::operator /= (const float32 rhs)
{
	_array[0] /= rhs;
	_array[1] /= rhs;
	_array[2] /= rhs;
	_array[3] /= rhs;
	_array[4] /= rhs;
	_array[5] /= rhs;
	_array[6] /= rhs;
	_array[7] /= rhs;
	_array[8] /= rhs;
	_array[9] /= rhs;
	_array[10] /= rhs;
	_array[11] /= rhs;
	_array[12] /= rhs;
	_array[13] /= rhs;
	_array[14] /= rhs;
	_array[15] /= rhs;
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
