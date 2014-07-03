#include "../Memory.h"
#include "Component.h"
#include "SceneGroup.h"
#include "Updatable.h"
#include "Tickable.h"
using namespace core;

Component::Component()
: ScriptObject(), mSceneNode(nullptr), mSceneGroup(nullptr), mTypeMask(0), mBoundingBox(AABB::UNIT)
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
	assert_not_null(node);
	assert_null(mSceneNode);
	assert_null(mSceneGroup);

	mSceneNode = node;
	mSceneGroup = node->GetSceneGroup();

	this->OnComponentAddedToNode();
}

void Component::ComponentRemovedFromNode(SceneNode* node)
{
	assert_not_null(node);
	assert(node == mSceneNode);

	this->OnComponentRemovedFromNode();

	mSceneNode = nullptr;
	mSceneGroup = nullptr;
}

void Component::ComponentAddedToGroup(SceneGroup* group)
{
	assert_not_null(group);
	assert_null(mSceneNode);
	assert_null(mSceneGroup);

	mSceneGroup = group;

	this->OnComponentAddedToGroup();
}

void Component::ComponentRemovedFromGroup(SceneGroup* group)
{
	assert_not_null(group);
	assert(group == mSceneGroup);

	this->OnComponentRemovedFromGroup();

	mSceneGroup = nullptr;
}

void Component::OnComponentAddedToNode()
{
}

void Component::OnComponentRemovedFromNode()
{
}

void Component::OnComponentAddedToGroup()
{
}

void Component::OnComponentRemovedFromGroup()
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
