#include <core/math/Matrix4x4.h>
#include "../test/Test.h"
using namespace core;

TEST_SUITE(Matrix4x4)
{
	UNIT_TEST(Matrix4x4_DefaultConstructorIsIdentityMatrix)
	{
		const Matrix4x4 value;
		ASSERT_EQUALS(value, Matrix4x4::IDENTITY);
	}

	UNIT_TEST(Matrix4x4_Translation)
	{
		const Vector3 t(10.0f, 20.0f, -30.0f);
		const Matrix4x4 value = Matrix4x4::Translation(t);

		const float32 expected[16] = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			t.x, t.y, t.z, 1.0f
		};

		ASSERT_EQUALS(value[0], expected[0]);
		ASSERT_EQUALS(value[1], expected[1]);
		ASSERT_EQUALS(value[2], expected[2]);
		ASSERT_EQUALS(value[3], expected[3]);
		ASSERT_EQUALS(value[4], expected[4]);
		ASSERT_EQUALS(value[5], expected[5]);
		ASSERT_EQUALS(value[6], expected[6]);
		ASSERT_EQUALS(value[7], expected[7]);
		ASSERT_EQUALS(value[8], expected[8]);
		ASSERT_EQUALS(value[9], expected[9]);
		ASSERT_EQUALS(value[10], expected[10]);
		ASSERT_EQUALS(value[11], expected[11]);
		ASSERT_EQUALS(value[12], expected[12]);
		ASSERT_EQUALS(value[13], expected[13]);
		ASSERT_EQUALS(value[14], expected[14]);
		ASSERT_EQUALS(value[15], expected[15]);
	}

	UNIT_TEST(Matrix4x4_RotationX)
	{
		const float32 a = 45.0f;
		const Matrix4x4 value = Matrix4x4::RotationX(a);

		const float32 expected[16] = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, cos(a), sin(a), 0.0f,
			0.0f, -sin(a), cos(a), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};

		ASSERT_EQUALS(value[0], expected[0]);
		ASSERT_EQUALS(value[1], expected[1]);
		ASSERT_EQUALS(value[2], expected[2]);
		ASSERT_EQUALS(value[3], expected[3]);
		ASSERT_EQUALS(value[4], expected[4]);
		ASSERT_EQUALS(value[5], expected[5]);
		ASSERT_EQUALS(value[6], expected[6]);
		ASSERT_EQUALS(value[7], expected[7]);
		ASSERT_EQUALS(value[8], expected[8]);
		ASSERT_EQUALS(value[9], expected[9]);
		ASSERT_EQUALS(value[10], expected[10]);
		ASSERT_EQUALS(value[11], expected[11]);
		ASSERT_EQUALS(value[12], expected[12]);
		ASSERT_EQUALS(value[13], expected[13]);
		ASSERT_EQUALS(value[14], expected[14]);
		ASSERT_EQUALS(value[15], expected[15]);
	}

	UNIT_TEST(Matrix4x4_RotationY)
	{
		const float32 a = 45.0f;
		const Matrix4x4 value = Matrix4x4::RotationY(a);

		const float32 expected[16] = {
			cos(a), 0.0f, -sin(a), 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			sin(a), 0.0f, cos(a), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};

		ASSERT_EQUALS(value[0], expected[0]);
		ASSERT_EQUALS(value[1], expected[1]);
		ASSERT_EQUALS(value[2], expected[2]);
		ASSERT_EQUALS(value[3], expected[3]);
		ASSERT_EQUALS(value[4], expected[4]);
		ASSERT_EQUALS(value[5], expected[5]);
		ASSERT_EQUALS(value[6], expected[6]);
		ASSERT_EQUALS(value[7], expected[7]);
		ASSERT_EQUALS(value[8], expected[8]);
		ASSERT_EQUALS(value[9], expected[9]);
		ASSERT_EQUALS(value[10], expected[10]);
		ASSERT_EQUALS(value[11], expected[11]);
		ASSERT_EQUALS(value[12], expected[12]);
		ASSERT_EQUALS(value[13], expected[13]);
		ASSERT_EQUALS(value[14], expected[14]);
		ASSERT_EQUALS(value[15], expected[15]);
	}

	UNIT_TEST(Matrix4x4_RotationZ)
	{
		const float32 a = 45.0f;
		const Matrix4x4 value = Matrix4x4::RotationZ(a);
		
		const float32 expected[16] = {
			cos(a), sin(a), 0.0f, 0.0f,
			-sin(a), cos(a), 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};

		ASSERT_EQUALS(value[0], expected[0]);
		ASSERT_EQUALS(value[1], expected[1]);
		ASSERT_EQUALS(value[2], expected[2]);
		ASSERT_EQUALS(value[3], expected[3]);
		ASSERT_EQUALS(value[4], expected[4]);
		ASSERT_EQUALS(value[5], expected[5]);
		ASSERT_EQUALS(value[6], expected[6]);
		ASSERT_EQUALS(value[7], expected[7]);
		ASSERT_EQUALS(value[8], expected[8]);
		ASSERT_EQUALS(value[9], expected[9]);
		ASSERT_EQUALS(value[10], expected[10]);
		ASSERT_EQUALS(value[11], expected[11]);
		ASSERT_EQUALS(value[12], expected[12]);
		ASSERT_EQUALS(value[13], expected[13]);
		ASSERT_EQUALS(value[14], expected[14]);
		ASSERT_EQUALS(value[15], expected[15]);
	}

	UNIT_TEST(Matrix4x4_RotateVector)
	{
		const Vector3 rotation(10.0f, 20.0f, 30.0f);
		const Matrix4x4 value = Matrix4x4::Rotation(rotation);
		const Matrix4x4 expected = Matrix4x4::RotationX(rotation.x) * Matrix4x4::RotationY(rotation.y) * Matrix4x4::RotationZ(rotation.z);

		ASSERT_EQUALS(value, expected);
	}

	UNIT_TEST(Matrix4x4_Scale)
	{
		const Vector3 s(1.0f, 2.0f, 3.0f);
		const Matrix4x4 value = Matrix4x4::Scale(s);

		const float32 expected[16] = {
			s.x, 0.0f, 0.0f, 0.0f,
			0.0f, s.y, 0.0f, 0.0f,
			0.0f, 0.0f, s.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};

		ASSERT_EQUALS(value[0], expected[0]);
		ASSERT_EQUALS(value[1], expected[1]);
		ASSERT_EQUALS(value[2], expected[2]);
		ASSERT_EQUALS(value[3], expected[3]);
		ASSERT_EQUALS(value[4], expected[4]);
		ASSERT_EQUALS(value[5], expected[5]);
		ASSERT_EQUALS(value[6], expected[6]);
		ASSERT_EQUALS(value[7], expected[7]);
		ASSERT_EQUALS(value[8], expected[8]);
		ASSERT_EQUALS(value[9], expected[9]);
		ASSERT_EQUALS(value[10], expected[10]);
		ASSERT_EQUALS(value[11], expected[11]);
		ASSERT_EQUALS(value[12], expected[12]);
		ASSERT_EQUALS(value[13], expected[13]);
		ASSERT_EQUALS(value[14], expected[14]);
		ASSERT_EQUALS(value[15], expected[15]);
	}

	UNIT_TEST(Matrix4x4_VerifyColumnMajorOrder)
	{
		const Matrix4x4 matrix(
			0.0f, 1.0f, 2.0f, 3.0f,
			4.0f, 5.0f, 6.0f, 7.0f,
			8.0f, 9.0f, 10.0f, 11.0f,
			12.0f, 13.0f, 14.0f, 15.0f
		);

		const float32* arr = matrix._array;

		ASSERT_EQUALS(arr[0], 0.0f);
		ASSERT_EQUALS(arr[1], 4.0f);
		ASSERT_EQUALS(arr[2], 8.0f);
		ASSERT_EQUALS(arr[3], 12.0f);

		ASSERT_EQUALS(arr[4], 1.0f);
		ASSERT_EQUALS(arr[5], 5.0f);
		ASSERT_EQUALS(arr[6], 9.0f);
		ASSERT_EQUALS(arr[7], 13.0f);

		ASSERT_EQUALS(arr[8], 2.0f);
		ASSERT_EQUALS(arr[9], 6.0f);
		ASSERT_EQUALS(arr[10], 10.0f);
		ASSERT_EQUALS(arr[11], 14.0f);

		ASSERT_EQUALS(arr[12], 3.0f);
		ASSERT_EQUALS(arr[13], 7.0f);
		ASSERT_EQUALS(arr[14], 11.0f);
		ASSERT_EQUALS(arr[15], 15.0f);
	}

	UNIT_TEST(Matrix4x4_CopyConstructor)
	{
		const Matrix4x4 matrix(
			0.0f, 1.0f, 2.0f, 3.0f,
			4.0f, 5.0f, 6.0f, 7.0f,
			8.0f, 9.0f, 10.0f, 11.0f,
			12.0f, 13.0f, 14.0f, 15.0f
			);

		const Matrix4x4 copy = matrix;

		ASSERT_EQUALS(matrix, copy);
	}

	UNIT_TEST(Matrix4x4_Translate)
	{
		const Vector3 t(10.0f, 20.0f, -30.0f);
		Matrix4x4 value = Matrix4x4::IDENTITY;
		value.Translate(t);

		const float32 expected[16] = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			t.x, t.y, t.z, 1
		};

		ASSERT_EQUALS(value[0], expected[0]);
		ASSERT_EQUALS(value[1], expected[1]);
		ASSERT_EQUALS(value[2], expected[2]);
		ASSERT_EQUALS(value[3], expected[3]);
		ASSERT_EQUALS(value[4], expected[4]);
		ASSERT_EQUALS(value[5], expected[5]);
		ASSERT_EQUALS(value[6], expected[6]);
		ASSERT_EQUALS(value[7], expected[7]);
		ASSERT_EQUALS(value[8], expected[8]);
		ASSERT_EQUALS(value[9], expected[9]);
		ASSERT_EQUALS(value[10], expected[10]);
		ASSERT_EQUALS(value[11], expected[11]);
		ASSERT_EQUALS(value[12], expected[12]);
		ASSERT_EQUALS(value[13], expected[13]);
		ASSERT_EQUALS(value[14], expected[14]);
		ASSERT_EQUALS(value[15], expected[15]);
	}

	UNIT_TEST(Matrix4x4_MultiplyWithMatrix4x4)
	{
		const Matrix4x4 A(
			1, 2, 3, 4,
			5, 6, 7, 8,
			9, 10, 11, 12,
			13, 14, 15, 16
		);

		const Matrix4x4 B(
			1, 2, 3, 4,
			5, 6, 7, 8,
			9, 10, 11, 12,
			13, 14, 15, 16
		);

		// http://onlinemschool.com/math/assistance/matrix/multiply/
		// Remember that the matrix is column major, which means that the
		// matrix is transposed in memory
		const float32 expected[16] = {
			90, 202, 314, 426, 
			100, 228, 356, 484,
			110, 254, 398, 542,
			120, 280, 440, 600
		};

		const Matrix4x4 value = A * B;

		ASSERT_EQUALS(value[0], expected[0]);
		ASSERT_EQUALS(value[1], expected[1]);
		ASSERT_EQUALS(value[2], expected[2]);
		ASSERT_EQUALS(value[3], expected[3]);
		ASSERT_EQUALS(value[4], expected[4]);
		ASSERT_EQUALS(value[5], expected[5]);
		ASSERT_EQUALS(value[6], expected[6]);
		ASSERT_EQUALS(value[7], expected[7]);
		ASSERT_EQUALS(value[8], expected[8]);
		ASSERT_EQUALS(value[9], expected[9]);
		ASSERT_EQUALS(value[10], expected[10]);
		ASSERT_EQUALS(value[11], expected[11]);
		ASSERT_EQUALS(value[12], expected[12]);
		ASSERT_EQUALS(value[13], expected[13]);
		ASSERT_EQUALS(value[14], expected[14]);
		ASSERT_EQUALS(value[15], expected[15]);
	}

	UNIT_TEST(Matrix4x4_MultiplyWithVector3)
	{
		const Matrix4x4 A(
			1, 2, 3, 4,
			5, 6, 7, 8,
			9, 10, 11, 12,
			13, 14, 15, 16
		);

		const Vector3 vec(1, 2, 3);
		const Vector3 value = A * vec;

		const float32 expected[3] = { 18, 46, 74 };

		ASSERT_EQUALS(value[0], expected[0]);
		ASSERT_EQUALS(value[1], expected[1]);
		ASSERT_EQUALS(value[2], expected[2]);
	}
}
