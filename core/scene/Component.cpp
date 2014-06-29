#include "../Memory.h"
#include "Component.h"
#include "SceneGroup.h"
#include "Updatable.h"
#include "Tickable.h"
using namespace core;

Component::Component()
: ScriptObject(), mSceneNode(nullptr), mTypeMask(0), mBoundingBox(AABB::UNIT)
{
}

Component::~Component()
{
}

void Component::SetComponentID(const std::string& id)
{
	mComponentID = id;
}

void Component::SetTypeMask(typemask typeMask)
{
	mTypeMask = typeMask;
}

void Component::ComponentAddedToNode(SceneNode* node)
{
	mSceneNode = node;
	this->OnComponentAddedToNode(node);
}

void Component::ComponentRemovedFromNode(SceneNode* node)
{
	assert(node == mSceneNode);
	mSceneNode = nullptr;
	this->OnComponentRemovedFromNode(node);
}

void Component::OnComponentAddedToNode(SceneNode* node)
{
}

void Component::OnComponentRemovedFromNode(SceneNode* node)
{
}

void Component::OnPositionChanged(const Vector3& newPosition)
{
	mBoundingBox.SetPosition(newPosition);
}

void Component::OnRotationChanged(const Vector3& newRotation)
{
	//mBoundingBox.SetRotation(newRotation);
}

void Component::OnScaleChanged(const Vector3& newScale)
{
	mBoundingBox.SetScale(newScale);
}

void Component::SetBoundingBox(const AABB& boundingBox)
{
	mBoundingBox = boundingBox;
}

void Component::SetBoundingBox(const AABB& boundingBox, const Vector3& position, const Vector3& rotation, const Vector3& scale)
{
	mBoundingBox = boundingBox;
	mBoundingBox.SetPositionRotationScale(position, rotation, scale);
}

void Component::FireEvent(const Event* evt)
{
	OnEventReceived(evt);
}

void Component::FireEvent(const Event* evt, typemask typeMask)
{
	if (!BIT_ISSET(mTypeMask, typeMask))
		return;

	OnEventReceived(evt);
}

void Component::OnEventReceived(const Event* evt)
{
}

void Component::AttachedToScene()
{
	this->OnAttachedToScene();
}

void Component::DetachedFromScene()
{
	this->OnDetachedFromScene();
}

void Component::OnAttachedToScene()
{
}

void Component::OnDetachedFromScene()
{
}
