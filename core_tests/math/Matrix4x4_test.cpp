#include <core/math/matrix4x4.h>
#include "../test/Test.h"
using namespace core;

TEST_SUITE(Matrix4x4)
{
	UNIT_TEST(Matrix4x4_DefaultConstructorIsIdentityMatrix)
	{
		const Matrix4x4 value;
		ASSERT_EQUALS(value, Matrix4x4::IDENTITY);
	}

	UNIT_TEST(Matrix4x4_RotateX)
	{
		const float32 rotation = 45.0f;
		const Matrix4x4 value = Matrix4x4::RotationX(rotation);
	}
}
