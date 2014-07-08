#include "../../Memory.h"
#include "Renderable.h"
#include "../SceneGroup.h"
#include "../tree/ISpatialTree.h"
using namespace core;

Renderable::Renderable()
: SpatialSceneNode()
{
}

Renderable::~Renderable()
{
}

void Renderable::UpdateModelMatrix()
{
	const Vector3& position = mAbsolutePosition;
	const Vector3& rotation = mAbsoluteRotation;
	const Vector3& scale = mAbsoluteScale;

	mModelMatrix = Matrix4x4::Translation(position);
	if (rotation.IsNotZero())
		mModelMatrix *= Matrix4x4::Rotation(rotation);
	if (scale.IsNotZero())
		mModelMatrix *= Matrix4x4::Scale(scale);
}

void Renderable::OnPositionChanged()
{
	SpatialSceneNode::OnPositionChanged();
	UpdateModelMatrix();
}

void Renderable::OnRotationChanged()
{
	SpatialSceneNode::OnRotationChanged();
	UpdateModelMatrix();
}

void Renderable::OnScaleChanged()
{
	SpatialSceneNode::OnScaleChanged();
	UpdateModelMatrix();
}

void Renderable::OnAddedToSceneGroup()
{
	SceneGroup* group = GetSceneGroup();
	group->AddRenderable(this);
}

void Renderable::OnRemovedFromSceneGroup()
{
	SceneGroup* group = GetSceneGroup();
	group->RemoveRenderable(this);
}
