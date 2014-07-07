#include "SpotLight.h"
using namespace core;

SpotLight::SpotLight()
: LightSource()
{

}

SpotLight::~SpotLight()
{

}

void SpotLight::OnAddedToSceneGroup()
{
	// Create a point-light buffer matching the radius of the object
	//if (mPointLightBuffer == nullptr)
	//	mPointLightBuffer = VertexBufferUtils::CreateSphere(70, 0, 0, 0, BufferUsage::DYNAMIC);	

	// TODO: Create a formula for calculating the bounding box based on the radius and light intensity.
	SetBoundingBox(AABB(Vector3::ZERO, mCutoff, mCutoff, mCutoff));
	LightSource::OnAddedToSceneGroup();
}

void SpotLight::CollectLightBlocks(const FindQuery& query, LightSourceResultSet* _out_resultSet) const
{
	auto block = _out_resultSet->Create();
	block->color = mColor;
	block->direction = mDirection;
	block->radius = (mCutoff * M_PI_F) / 180.0f;
}
