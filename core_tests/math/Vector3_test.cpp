#include <core/math/vector3.h>
#include "../test/Test.h"
using namespace core;

TEST_SUITE(Vector3)
{
	UNIT_TEST(Vector3_DefaultConstructor)
	{
		const Vector3 val;

		ASSERT_EQUALS(val, Vector3::ZERO);
	}

	UNIT_TEST(Vector3_ConstructorElements)
	{
		const float32 x = 3.0f;
		const float32 y = 5.0f;
		const float32 z = -201.043f;
		const Vector3 val(x, y, z);

		ASSERT_EQUALS(val.x, x);
		ASSERT_EQUALS(val.y, y);
		ASSERT_EQUALS(val.z, z);
	}

	UNIT_TEST(Vector3_CopyConstructor)
	{
		const Vector3 expected(3.0f, 5.0f, -201.043f);
		const Vector3 val = expected;
	
		ASSERT_EQUALS(val.x, expected.x);
		ASSERT_EQUALS(val.y, expected.y);
		ASSERT_EQUALS(val.z, expected.z);
	}

	UNIT_TEST(Vector3_Add)
	{
		const Vector3 val1(1.0f, 2.0f, 3.0f);
		const Vector3 val2(3.0f, 4.0f, 5.0f);
		const Vector3 expected(4.0f, 6.0f, 8.0f);

		ASSERT_TRUE(val1 + val2 == expected);
	}

	UNIT_TEST(Vector3_Sub)
	{
		const Vector3 val1(2.0f, 4.0f, 6.0f);
		const Vector3 val2(3.0f, 1.0f, 2.0f);
		const Vector3 expected(-1.0f, 3.0f, 4.0f);

		ASSERT_TRUE(val1 - val2 == expected);
	}

	UNIT_TEST(Vector3_Div)
	{
		const Vector3 val1(2.0f, 12.0f, 1.0f);
		const float32 val2 = 4.0f;
		const Vector3 expected(0.5f, 3.0f, 0.25f);

		ASSERT_TRUE(val1 / val2 == expected);
	}

	UNIT_TEST(Vector3_Mul)
	{
		const Vector3 val1(2.0f, 12.0f, 1.0f);
		const Vector3 val2(2.0f, 2.0f, 2.0f);
		const Vector3 expected(4.0f, 24.0f, 2.0f);

		ASSERT_TRUE(val1 * val2 == expected);
	}

	UNIT_TEST(Vector3_Length)
	{
		const Vector3 val1(3.0f, 4.0f, 5.0f);
		const float32 expected = sqrtf(3*3 + 4*4 + 5*5);

		ASSERT_EQUALS(val1.Length(), expected);
	}

	UNIT_TEST(Vector3_SquaredLength)
	{
		const Vector3 val1(3.0f, 4.0f, 5.0f);
		const float32 expected = 3*3 + 4*4 + 5*5;

		ASSERT_EQUALS(val1.SquaredLength(), expected);
	}

	UNIT_TEST(Vector3_Normalize)
	{
		Vector3 �(3.0f, 4.0f, 5.0f);
		�.Normalize();
		const Vector3 expected = � / �.Length();

		ASSERT_EQUALS(�, expected);
	}

	UNIT_TEST(Vector3_ZeroAssignment)
	{
		Vector3 val1(3.0f, 4.0f, 5.0f);
		val1 = Vector3::ZERO;

		ASSERT_EQUALS(val1, Vector3::ZERO);
	}

	UNIT_TEST(Vector3_Negate)
	{
		Vector3 val1(3.0f, 4.0f, 5.0f);
		val1.Negate();
		const Vector3 expected(-3.0f, -4.0f, -5.0f);

		ASSERT_EQUALS(val1, expected);
	}

	UNIT_TEST(Vector3_Equals)
	{
		const Vector3 val1(3.542f, 12.219f, -32.012f);
		const Vector3 val2(3.542f, 12.219f, -32.012f);
	
		ASSERT_TRUE(val1 == val2);
	}

	UNIT_TEST(Vector3_NotEquals)
	{
		const Vector3 val1(3.5f, 12.21f, -32.012f);
		const Vector3 val2(3.51f, 12.21f, -32.01f);
	
		ASSERT_TRUE(val1 != val2);
	}

	UNIT_TEST(Vector3_CrossProduct)
	{
		const Vector3 val1(3.5f, 12.21f, -32.012f);
		const Vector3 val2(3.51f, 12.21f, -32.01f);

		const Vector3 expected(
			val1.y * val2.z - val1.z * val2.y,
			val1.z * val2.x - val1.x * val2.z,
			val1.x * val2.y - val1.y * val2.x
		);

		ASSERT_EQUALS(val1.CrossProduct(val2), expected);
	}
}
