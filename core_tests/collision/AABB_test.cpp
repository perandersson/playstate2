#include <core/collision/AABB.h>
#include "../test/Test.h"
using namespace core;

TEST_SUITE(AABB)
{
	UNIT_TEST(AABB_DefaultConstructor)
	{
		AABB boundingBox;

		ASSERT_TRUE(boundingBox.minPoint.IsZero());
		ASSERT_TRUE(boundingBox.maxPoint.IsZero());
		ASSERT_TRUE(boundingBox.offset.IsZero());
	}

	UNIT_TEST(AABB_SetConstructor)
	{
		const Vector3 position(50.f, 50.f, 50.f);
		const float32 width = 2.0f;
		const float32 height = 2.0f;
		const float32 depth = 2.0f;
		AABB boundingBox(position, width, height, depth);

		ASSERT_EQUALS(boundingBox.minPoint, Vector3(position.x - width / 2.0f, position.y - height / 2.0f, position.z - depth / 2.0f));
		ASSERT_EQUALS(boundingBox.maxPoint, Vector3(position.x + width / 2.0f, position.y + height / 2.0f, position.z + depth / 2.0f));
		ASSERT_EQUALS(boundingBox.offset, position);
	}

	UNIT_TEST(AABB_CopyConstructor)
	{
		AABB boundingBox(AABB::UNIT);

		ASSERT_EQUALS(boundingBox.minPoint, Vector3(-0.5f, -0.5f, -0.5f));
		ASSERT_EQUALS(boundingBox.maxPoint, Vector3(0.5f, 0.5f, 0.5f));
		ASSERT_TRUE(boundingBox.offset.IsZero());
	}

	UNIT_TEST(AABB_SetPositionRotationScale)
	{
		const float32 width = 2.0f;
		const float32 height = 2.0f;
		const float32 depth = 2.0f;
		AABB boundingBox(Vector3::ZERO, width, height, depth);

		ASSERT_EQUALS(boundingBox.minPoint, Vector3(-1.0f * width / 2.0f, -1.0f * height / 2.0f, -1.0f * depth / 2.0f));
		ASSERT_EQUALS(boundingBox.maxPoint, Vector3(width / 2.0f, height / 2.0f, depth / 2.0f));
		ASSERT_TRUE(boundingBox.offset.IsZero());

		const Vector3 position(50.f, 50.f, 50.f);
		const Vector3 rotation(0.0f, 0.0f, 0.0f);
		const Vector3 scale(2.0f, 2.0f, 2.0f);

		boundingBox.SetPositionRotationScale(position, rotation, scale);

		ASSERT_EQUALS(boundingBox.minPoint, Vector3(position.x - (width / 2.0f) * scale.x, position.y - (height / 2.0f) * scale.y, position.z - (depth / 2.0f) * scale.z));
		ASSERT_EQUALS(boundingBox.maxPoint, Vector3(position.x + (width / 2.0f) * scale.x, position.y + (height / 2.0f) * scale.y, position.z + (depth / 2.0f) * scale.z));
		ASSERT_TRUE(boundingBox.offset.IsZero());

	}

}
