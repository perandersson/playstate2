#include <core/camera/CameraUtils.h>
#include "../test/Test.h"
using namespace core;

TEST_SUITE(Camera)
{
	UNIT_TEST(CameraUtils_GetPerspective)
	{
		const float32 near = 1.0f;
		const float32 far = 200.0f;
		const float32 fov = 45.0f;
		const float32 ratio = (1024.0f / 768.0f);

		const Matrix4x4 value = CameraUtils::GetPerspective(near, far, fov, ratio);

		const float32 top = near * tanf(fov * (float32)(M_PI / 360.0));
		const float32 bottom = -top;
		const float32 right = top * ratio;
		const float32 left = -right;

		const float32 expected[16] = {
			(2.0f * near) / (right - left), 0, 0, 0,
			0, (2.0f * near) / (top - bottom), 0, 0,
			(right + left) / (right - left), (top + bottom) / (top - bottom), -((far + near) / (far - near)), -1.0f,
			0, 0, -((2.0f * far * near) / (far - near)), 0
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

	UNIT_TEST(CameraUtils_GetOrtho2D) {
		const float32 near = -1;
		const float32 far = 1;

		const float32 left = -1.0f;
		const float32 right = 1.0f;
		const float32 bottom = -1.0f;
		const float32 top = 1.0f;

		const Matrix4x4 value = CameraUtils::GetOrtho2D(left, right, bottom, top);

		const float32 expected[16] = {
			2.0f / (right - left), 0, 0, 0,
			0, 2.0f / (top - bottom), 0, 0,
			0, 0, -(2.0f / (far - near)), 0,
			-((right + left) / (right - left)), -((top + bottom) / (top - bottom)), -((far + near) / (far - near)), 1.0f
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
}
