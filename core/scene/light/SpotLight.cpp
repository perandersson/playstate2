#include "../../Memory.h"
#include "SpotLight.h"
#include "../../camera/Camera.h"
using namespace core;

SpotLight::SpotLight()
: LightSource(), mSpotLightCone(nullptr), mSpotExponent(2.0f)
{

}

SpotLight::~SpotLight()
{
	if (mSpotLightCone != nullptr) {
		delete mSpotLightCone;
		mSpotLightCone = nullptr;
	}
}

void SpotLight::SetCutoff(float32 cutoff)
{
	mCutoff = cutoff;
	UpdateShape();
}

void SpotLight::SetSpotDirection(const Vector3& spotDirection)
{
	mSpotDirection = spotDirection;
	UpdateShape();
}

void SpotLight::OnAddedToSceneGroup()
{
	// TODO: Create a formula for calculating the bounding box based on the radius and light intensity.
	SetBoundingBox(AABB(Vector3::ZERO, mCutoff * 2, mCutoff * 2, mCutoff * 2));
	LightSource::OnAddedToSceneGroup();

	// Create a shape that contains the entire spotlight
	Vector3 direction = mSpotDirection - mAbsolutePosition;
	float32 height = direction.Length();
	direction /= height;
	mSpotLightCone = Cone::Create(Vector3::ZERO, direction, height, mCutoff, 30U, BufferUsage::DYNAMIC);

	// Update light projector's view perspective
	mProjector.SetPerspective(1.f, height, mCutoff * 2.0f, 1.0f);
	mProjector.LookAt(mAbsolutePosition, direction, Vector3(0, 1, 0));
}

void SpotLight::CollectLightBlocks(const FindQuery& query, LightSourceResultSet* _out_resultSet) const
{
	if (BIT_ISSET(query.filter, FindQueryFilter::SPOT_LIGHTS)) {
		auto block = _out_resultSet->Create(GetUID());
		block->position = mAbsolutePosition;
		block->color = mColor;
		block->direction = mSpotLightCone->GetDirection();
		block->spotExponent = mSpotExponent;
		block->radius = mCutoff;
		block->projector = &mProjector;

		if (BIT_ISSET(query.filter, FindQueryFilter::TEXTURES)) {
			block->texture = mTexture.Get();
		}

		if (BIT_ISSET(query.filter, FindQueryFilter::GEOMETRY)) {
			block->vertexBuffer = mSpotLightCone->GetVertexBuffer();
			block->indexBuffer = mSpotLightCone->GetIndexBuffer();
		}
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

		// Update light projector's view perspective
		mProjector.SetPerspective(1.f, height, mCutoff * 2.0f, 1.0f);
		mProjector.LookAt(mAbsolutePosition, direction, Vector3(0, 1, 0));
	}
}
