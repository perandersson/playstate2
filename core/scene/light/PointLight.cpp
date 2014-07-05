#include "../../Memory.h"
#include "PointLight.h"
#include "../../rendering/VertexBufferUtils.h"
#include "../../StringUtils.h"
#include <cmath>
using namespace core;

PointLight::PointLight()
: LightSource(), mColor(Color::WHITE), mRadius(1.0f), mConstantAttenuation(0.0f), mLinearAttenuation(0.0f), mQuadricAttenuation(0.0f), mPointLightBuffer(nullptr)
{
}

PointLight::~PointLight()
{
	if (mPointLightBuffer != nullptr) {
		delete mPointLightBuffer;
		mPointLightBuffer = nullptr;
	}
}

void PointLight::SetOffset(const Vector3& offset)
{
	mOffset = offset;
}

void PointLight::SetColor(const Color& color)
{
	mColor = color;
}

void PointLight::SetRadius(float32 radius)
{
	mRadius = radius;
}

void PointLight::SetConstantAttenuation(float32 constantAttenuation)
{
	mConstantAttenuation = constantAttenuation;
}

void PointLight::SetLinearAttenuation(float32 linearAttenuation)
{
	mLinearAttenuation = linearAttenuation;
}

void PointLight::SetQuadricAttenuation(float32 quadricAttenuation)
{
	mQuadricAttenuation = quadricAttenuation;
}

void PointLight::OnAddedToSceneGroup()
{
	// Create a point-light buffer matching the radius of the object
	//if (mPointLightBuffer == nullptr)
	//	mPointLightBuffer = VertexBufferUtils::CreateSphere(70, 0, 0, 0, BufferUsage::DYNAMIC);	

	// TODO: Create a formula for calculating the bounding box based on the radius and light intensity.
	SetBoundingBox(AABB(Vector3::ZERO, 1.3f * mRadius, 1.3f * mRadius, 1.3f * mRadius));
	LightSource::OnAddedToSceneGroup();
}

void PointLight::CollectLightBlocks(const FindQuery& query, LightSourceResultSet* _out_resultSet) const
{
	auto block = _out_resultSet->Create();
	block->position = GetAbsolutePosition() + mOffset;
	block->color = mColor;
	block->radius = mRadius;
	block->constantAttenuation = mConstantAttenuation;
	block->linearAttenuation = mLinearAttenuation;
	block->quadricAttenuation = mQuadricAttenuation;
	block->vertexBuffer = mPointLightBuffer;
}
