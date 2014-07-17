#include "../../Memory.h"
#include "SpotLight.h"
using namespace core;

SpotLight::SpotLight()
: LightSource(), mSpotLightCone(nullptr)
{

}

SpotLight::~SpotLight()
{
	if (mSpotLightCone != nullptr) {
		delete mSpotLightCone;
		mSpotLightCone = nullptr;
	}
}

void SpotLight::OnAddedToSceneGroup()
{
	// Create a point-light buffer matching the radius of the object
	//if (mPointLightBuffer == nullptr)
	//	mPointLightBuffer = VertexBufferUtils::CreateSphere(70, 0, 0, 0, BufferUsage::DYNAMIC);	

	// TODO: Create a formula for calculating the bounding box based on the radius and light intensity.
	SetBoundingBox(AABB(Vector3::ZERO, mCutoff * 2, mCutoff * 2, mCutoff * 2));
	LightSource::OnAddedToSceneGroup();

	// Create a shape that contains the entire spotlight
	Vector3 direction = mSpotDirection - mAbsolutePosition;
	float32 height = direction.Length();
	direction /= height;
	mSpotLightCone = Cone::Create(Vector3::ZERO, direction, height, mCutoff, 30U, BufferUsage::DYNAMIC);
}

void SpotLight::CollectLightBlocks(const FindQuery& query, LightSourceResultSet* _out_resultSet) const
{
	if (BIT_ISSET(query.filter, FindQueryFilter::SPOT_LIGHTS)) {
		auto block = _out_resultSet->Create();
		block->position = GetAbsolutePosition();
		block->color = mColor;
		block->direction = mSpotLightCone->GetDirection();
		block->radius = mCutoff;
		block->vertexBuffer = mSpotLightCone->GetVertexBuffer();
		block->indexBuffer = mSpotLightCone->GetIndexBuffer();
	}
}

void SpotLight::OnPositionChanged()
{
	LightSource::OnPositionChanged();
	UpdateShape();
}

void SpotLight::OnRotationChanged()
{
	LightSource::OnRotationChanged();
	UpdateShape();
}

void SpotLight::OnScaleChanged()
{
	LightSource::OnScaleChanged();
	UpdateShape();
}

void SpotLight::UpdateShape()
{
	// Update the spotlight's shape
	if (mSpotLightCone != nullptr) {
		Vector3 direction = mSpotDirection - mAbsolutePosition;
		float32 height = direction.Length();
		direction /= height;

		mSpotLightCone->Update(Vector3::ZERO, direction, height, mCutoff, 30U);
	}
}
