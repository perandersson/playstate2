#include "../Memory.h"
#include "AABB.h"
using namespace core;

core::AABB core::AABB::UNIT(Vector3::ZERO, 1, 1, 1);

AABB::AABB()
{
}

AABB::AABB(const Vector3& position, float32 width, float32 height, float32 depth)
: offset(position)
{
	Set(position, width, height, depth);
}

AABB::AABB(const AABB& box)
: minPoint(box.minPoint), maxPoint(box.maxPoint), offset(box.offset)
{
}

AABB::~AABB()
{
}

void AABB::SetPosition(const Vector3& position)
{
	const float32 width = maxPoint.x - minPoint.x;
	const float32 height = maxPoint.y - minPoint.y;
	const float32 depth = maxPoint.z - minPoint.z;

	Set(position + offset, width, height, depth);
}

void AABB::SetScale(const Vector3& scale)
{
	const float32 width = maxPoint.x - minPoint.x;
	const float32 height = maxPoint.y - minPoint.y;
	const float32 depth = maxPoint.z - minPoint.z;

	Set(GetPosition() - offset, width * scale.x, height * scale.y, depth * scale.z);
}

void AABB::Translate(const Vector3& direction)
{
	float32 width = maxPoint.x - minPoint.x;
	float32 height = maxPoint.y - minPoint.y;
	float32 depth = maxPoint.z - minPoint.z;

	Set(GetPosition() + offset + direction, width, height, depth);
}

void AABB::Scale(const Vector3& scale)
{
	maxPoint.x *= scale.x; minPoint.x *= scale.x;
	maxPoint.y *= scale.y; minPoint.y *= scale.y;
	maxPoint.z *= scale.z; minPoint.z *= scale.z;
}

void AABB::SetPositionRotationScale(const Vector3& position, const Vector3& rotation, const Vector3& scale)
{
	const float32 width = maxPoint.x - minPoint.x;
	const float32 height = maxPoint.y - minPoint.y;
	const float32 depth = maxPoint.z - minPoint.z;

	// TODO: Fix scale support (scaling will result in an infinte sized bounding box - eventually)
	assert(scale.x == 1.0f);
	assert(scale.y == 1.0f);
	assert(scale.z == 1.0f);

	// TODO: Fix rotation support for AABB
	//assert(rotation.IsZero() && "No rotation support yet!");

	Set(position + offset, width * scale.x, height * scale.y, depth * scale.z);
}

void AABB::SetWidth(float32 width)
{
	float32 height = maxPoint.y - minPoint.y;
	float32 depth = maxPoint.z - minPoint.z;

	Set(GetPosition(), width, height, depth);
}

void AABB::SetHeight(float32 height)
{
	float32 width = maxPoint.x - minPoint.x;
	float32 depth = maxPoint.z - minPoint.z;

	Set(GetPosition(), width, height, depth);
}

void AABB::SetDepth(float32 depth)
{
	float32 width = maxPoint.x - minPoint.x;
	float32 height = maxPoint.y - minPoint.y;

	Set(GetPosition(), width, height, depth);
}

Vector3 AABB::GetPosition() const
{
	return ((maxPoint + minPoint) / 2.0f);
}

float32 AABB::GetWidth() const
{
	return maxPoint.x - minPoint.x;
}

float32 AABB::GetHeight() const
{
	return maxPoint.y - minPoint.y;
}

float32 AABB::GetDepth() const
{
	return maxPoint.z - minPoint.z;
}

CollisionResult::Enum AABB::IsColliding(const AABB& otherBox) const
{
	const Vector3& otherMin = otherBox.minPoint;
	const Vector3& otherMax = otherBox.maxPoint;

	// The chance of NOT colliding is bigger than either contains or intersects.

	if (maxPoint.x < otherMin.x || minPoint.x > otherMax.x)
		return CollisionResult::OUTSIDE;

	if (maxPoint.y < otherMin.y || minPoint.y > otherMax.y)
		return CollisionResult::OUTSIDE;

	if (maxPoint.z < otherMin.z || minPoint.z > otherMax.z)
		return CollisionResult::OUTSIDE;

	if (otherMin.x >= minPoint.x && otherMax.x <= maxPoint.x &&
		otherMin.y >= minPoint.y && otherMax.y <= maxPoint.y &&
		otherMin.z >= minPoint.z && otherMax.z <= maxPoint.z) {
		return CollisionResult::CONTAINED;
	}

	return CollisionResult::INTERSECT;
}

void AABB::Set(const Vector3& position, float32 width, float32 height, float32 depth)
{
	assert(width >= 0.0f && "you are not allowed to use negative value for the bounding box width");
	assert(height >= 0.0f && "you are not allowed to use negative value for the bounding box height");
	assert(depth >= 0.0f && "you are not allowed to use negative value for the bounding box depth");

	float32 halfWidth = width * 0.5f;
	float32 halfHeight = height * 0.5f;
	float32 halfDepth = depth * 0.5f;

	maxPoint = position;
	maxPoint.x += halfWidth;
	maxPoint.y += halfHeight;
	maxPoint.z += halfDepth;

	minPoint = position;
	minPoint.x -= halfWidth;
	minPoint.y -= halfHeight;
	minPoint.z -= halfDepth;
}

AABB& AABB::operator = (const AABB& box)
{
	maxPoint = box.maxPoint;
	minPoint = box.minPoint;
	offset = box.offset;
	return *this;
}
