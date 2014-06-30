#include "../Memory.h"
#include "SceneNode.h"
#include "SceneGroup.h"
#include "Component.h"
#include "rendering/Renderable.h"
using namespace core;

SceneNode::SceneNode()
: ScriptObject(), mParent(nullptr), mSceneGroup(nullptr), mTypeMask(0), mScale(Vector3::ONE), mAbsoluteScale(Vector3::ONE),
mChildren(offsetof(SceneNode, SceneNodeLink)), mComponents(offsetof(Component, ComponentLink))
{
}

SceneNode::~SceneNode()
{
	mChildren.DeleteAll();
	mComponents.DeleteAll();
	mParent = nullptr;
}

void SceneNode::AddChildNode(SceneNode* node)
{
	assert_not_null(node);

	mChildren.AddLast(node);
	node->mParent = this;
	node->mSceneGroup = mSceneGroup;
	node->UpdatePosition(mAbsolutePosition);
	node->UpdateRotation(mAbsoluteRotation);
	node->UpdateScale(mAbsoluteScale);
	this->OnSceneNodeAdded(node);
	node->OnAttachedToParent(this);
	if (IsAttachedToScene())
		this->AttachedToScene();
}

void SceneNode::RemoveChildNode(SceneNode* node)
{
	assert_not_null(node);
	assert(node->mParent == this && "You are not allowed to remove another scene objects child node");

	mChildren.Remove(node);
	node->mParent = nullptr;
	this->OnSceneNodeRemoved(node);
	node->OnDetachedFromParent(this);
	if (IsAttachedToScene())
		node->DetachedFromScene();
}

void SceneNode::RemoveFromParent()
{
	if (mParent == nullptr)
		return;

	mParent->RemoveChildNode(this);
}

void SceneNode::AttachedToScene()
{
	this->OnAttachedToScene();

	auto component = mComponents.First();
	while (component != nullptr) {
		auto next = component->ComponentLink.Tail;
		component->AttachedToScene();
		component = next;
	}

	auto node = mChildren.First();
	while (node != nullptr) {
		auto next = node->SceneNodeLink.Tail;
		node->AttachedToScene();
		node = next;
	}
}

void SceneNode::DetachedFromScene()
{
	auto node = mChildren.First();
	while (node != nullptr) {
		auto next = node->SceneNodeLink.Tail;
		node->DetachedFromScene();
		node = next;
	}

	auto component = mComponents.First();
	while (component != nullptr) {
		auto next = component->ComponentLink.Tail;
		component->DetachedFromScene();
		component = next;
	}

	this->OnDetachedFromScene();
}

SceneNode* SceneNode::FindSceneNode(const std::string& id)
{
	if (mID == id)
		return this;

	auto node = mChildren.First();
	while (node != nullptr) {
		auto next = node->SceneNodeLink.Tail;
		auto found = node->FindSceneNode(id);
		if (found != nullptr)
			return found;

		node = next;
	}

	return nullptr;
}

SceneNode* SceneNode::FindFirstSceneNode(typemask typeMask)
{
	if (BIT_ISSET(mTypeMask, typeMask))
		return this;

	auto node = mChildren.First();
	while (node != nullptr) {
		auto next = node->SceneNodeLink.Tail;
		auto found = node->FindFirstSceneNode(typeMask);
		if (found != nullptr)
			return found;

		node = node;
	}

	return nullptr;
}

bool SceneNode::IsAttachedToScene() const {
	if (mSceneGroup != nullptr)
		return mSceneGroup->IsAttachedToScene();
	return false;
}

void SceneNode::AddComponent(Component* component)
{
	assert_not_null(component);
	assert(component->GetSceneNode() == nullptr && "You are not allowed to add a component on multiple scene nodes");

	//
	// Add the component to it's internal list
	//

	mComponents.AddLast(component);
	component->ComponentAddedToNode(this);
	
	//
	// Add the components internal objects as well
	// 

	auto updatable = component->GetUpdatable();
	if (updatable != nullptr) {
		AddUpdatable(updatable);
	}

	auto tickable = component->GetTickable();
	if (tickable != nullptr) {
		AddTickable(tickable);
	}

	//auto renderable = component->GetRenderable();
	//if (renderable != nullptr) {
	//	AddRenderable(renderable);
	//}

	if (IsAttachedToScene())
		component->AttachedToScene();
}

void SceneNode::RemoveComponent(Component* component)
{
	assert_not_null(component);
	assert(component->GetSceneNode() == this && "You are not allowed to remove a component on that isn't attached here");
	
	auto updatable = component->GetUpdatable();
	if (updatable != nullptr) {
		RemoveUpdatable(updatable);
	}

	auto tickable = component->GetTickable();
	if (tickable != nullptr) {
		RemoveTickable(tickable);
	}

	//auto renderable = component->GetRenderable();
	//if (renderable != nullptr) {
	//	RemoveRenderable(renderable);
	//}

	mComponents.Remove(component);

	if (IsAttachedToScene())
		component->DetachedFromScene();

	component->ComponentRemovedFromNode(this);
}

Component* SceneNode::GetFirstComponent(typemask typeMask)
{
	auto component = mComponents.First();
	while (component != nullptr) {
		auto next = component->ComponentLink.Tail;
		if (BIT_ISSET(component->GetTypeMask(), typeMask))
			return component;

		component = next;
	}

	auto node = mChildren.First();
	while (node != nullptr) {
		auto next = node->SceneNodeLink.Tail;
		component = node->GetFirstComponent(typeMask);
		if (component != nullptr)
			return component;

		node = next;
	}

	return nullptr;
}

void SceneNode::SetTypeMask(typemask typeMask)
{
	mTypeMask = typeMask;
}

void SceneNode::AddedToSceneGroup(SceneGroup* group)
{
	mSceneGroup = group;
	auto component = mComponents.First();
	while (component != nullptr) {
		auto next = component->ComponentLink.Tail;

		auto updatable = component->GetUpdatable();
		if (updatable != nullptr) {
			AddUpdatable(updatable);
		}

		auto tickable = component->GetTickable();
		if (tickable != nullptr) {
			AddTickable(tickable);
		}

		//auto renderable = component->GetRenderable();
		//if (renderable != nullptr) {
		//	AddRenderable(renderable);
		//}
		//
		component = next;
	}

	if (group->IsAttachedToScene())
		this->AttachedToScene();

	this->OnAddedToSceneGroup();

	auto node = mChildren.First();
	while (node != nullptr) {
		auto next = node->SceneNodeLink.Tail;
		node->AddedToSceneGroup(group);
		node = next;
	}
}

void SceneNode::RemovedFromSceneGroup(SceneGroup* group)
{
	auto component = mComponents.First();
	while (component != nullptr) {
		auto next = component->ComponentLink.Tail;

		auto updatable = component->GetUpdatable();
		if (updatable != nullptr) {
			RemoveUpdatable(updatable);
		}

		auto tickable = component->GetTickable();
		if (tickable != nullptr) {
			RemoveTickable(tickable);
		}

		//auto renderable = component->GetRenderable();
		//if (renderable != nullptr) {
		//	RemoveRenderable(renderable);
		//}

		component = next;
	}

	if (group->IsAttachedToScene())
		this->DetachedFromScene();

	auto node = mChildren.First();
	while (node != nullptr) {
		auto next = node->SceneNodeLink.Tail;
		node->RemovedFromSceneGroup(group);
		node = next;
	}

	this->OnRemovedFromSceneGroup();
	mSceneGroup = nullptr;
}

void SceneNode::OnSceneNodeAdded(SceneNode* node)
{
}

void SceneNode::OnSceneNodeRemoved(SceneNode* node)
{
}

void SceneNode::OnDetachedFromParent(SceneNode* parent)
{
}

void SceneNode::OnAttachedToParent(SceneNode* parent)
{
}

void SceneNode::OnAddedToSceneGroup()
{
	mOnAddedToSceneGroupPtr->Invoke();
}

void SceneNode::OnRemovedFromSceneGroup()
{
	mOnRemovedFromSceneGroupPtr->Invoke();
}

void SceneNode::OnAttachedToScene()
{
	mOnAttachedToSceneMethodPtr->Invoke();
}

void SceneNode::OnDetachedFromScene()
{
	mOnDetachedFromSceneMethodPtr->Invoke();
}

void SceneNode::AddUpdatable(Updatable* updatable)
{
	assert_not_null(updatable);
	if (IsAttachedToSceneGroup())
		mSceneGroup->AddUpdatable(updatable);
}

void SceneNode::RemoveUpdatable(Updatable* updatable)
{
	assert_not_null(updatable);
	if (IsAttachedToSceneGroup())
		mSceneGroup->RemoveUpdatable(updatable);
}

void SceneNode::AddTickable(Tickable* tickable)
{
	assert_not_null(tickable);
	if (IsAttachedToSceneGroup())
		mSceneGroup->AddTickable(tickable);
}

void SceneNode::RemoveTickable(Tickable* tickable)
{
	assert_not_null(tickable);
	if (IsAttachedToSceneGroup())
		mSceneGroup->RemoveTickable(tickable);
}

void SceneNode::SetID(const std::string& id)
{
	mID = id;
}

void SceneNode::SetPosition(const Vector3& position)
{
	const Vector3 diff = position - mPosition;
	mPosition = position;
	mAbsolutePosition += diff;

	this->OnPositionChanged();

	auto component = mComponents.First();
	while (component != nullptr) {
		auto next = component->ComponentLink.Tail;
		component->OnPositionChanged(mAbsolutePosition);
		component = next;
	}

	auto child = mChildren.First();
	while (child != nullptr) {
		auto next = child->SceneNodeLink.Tail;
		child->UpdatePosition(mAbsolutePosition);
		child = next;
	}
}

void SceneNode::SetRotation(const Vector3& rotation)
{
	const Vector3 diff = rotation - mRotation;
	mRotation = rotation;
	mAbsoluteRotation += diff;

	this->OnRotationChanged();

	auto component = mComponents.First();
	while (component != nullptr) {
		auto next = component->ComponentLink.Tail;
		component->OnRotationChanged(mAbsoluteRotation);
		component = next;
	}

	auto child = mChildren.First();
	while (child != nullptr) {
		auto next = child->SceneNodeLink.Tail;
		child->UpdateRotation(mAbsoluteRotation);
		child = next;
	}
}

void SceneNode::SetScale(const Vector3& scale)
{
	const Vector3 diff = scale - mScale;
	mScale = scale;
	mAbsoluteScale += diff;

	this->OnScaleChanged();

	auto component = mComponents.First();
	while (component != nullptr) {
		auto next = component->ComponentLink.Tail;
		component->OnScaleChanged(mAbsoluteScale);
		component = next;
	}

	auto child = mChildren.First();
	while (child != nullptr) {
		auto next = child->SceneNodeLink.Tail;
		child->UpdateScale(mAbsoluteScale);
		child = next;
	}
}

void SceneNode::UpdatePosition(const Vector3& parentPosition)
{
	mAbsolutePosition = parentPosition + mPosition;

	this->OnPositionChanged();

	auto component = mComponents.First();
	while (component != nullptr) {
		auto next = component->ComponentLink.Tail;
		component->OnPositionChanged(mAbsolutePosition);
		component = next;
	}

	auto child = mChildren.First();
	while (child != NULL) {
		auto next = child->SceneNodeLink.Tail;
		child->UpdatePosition(mAbsolutePosition);
		child = next;
	}
}

void SceneNode::UpdateRotation(const Vector3& parentRotation)
{
	mAbsoluteRotation = parentRotation + mRotation;

	this->OnRotationChanged();

	auto component = mComponents.First();
	while (component != nullptr) {
		auto next = component->ComponentLink.Tail;
		component->OnRotationChanged(mAbsoluteRotation);
		component = next;
	}

	auto child = mChildren.First();
	while (child != NULL) {
		auto next = child->SceneNodeLink.Tail;
		child->UpdateRotation(mAbsoluteRotation);
		child = next;
	}
}

void SceneNode::UpdateScale(const Vector3& parentScale)
{
	mAbsoluteScale = parentScale + mScale;

	this->OnScaleChanged();

	auto component = mComponents.First();
	while (component != nullptr) {
		auto next = component->ComponentLink.Tail;
		component->OnScaleChanged(mAbsoluteScale);
		component = next;
	}

	auto child = mChildren.First();
	while (child != NULL) {
		auto next = child->SceneNodeLink.Tail;
		child->UpdateScale(mAbsoluteScale);
		child = next;
	}
}

void SceneNode::FireEvent(const Event* evt)
{
	auto component = mComponents.First();
	while (component != NULL) {
		auto next = component->ComponentLink.Tail;
		component->FireEvent(evt);
		component = next;
	}

	auto child = mChildren.First();
	while (child != NULL) {
		auto next = child->SceneNodeLink.Tail;
		child->FireEvent(evt);
		child = next;
	}

	OnEventReceived(evt);
}

void SceneNode::FireEvent(const Event* evt, typemask typeMask)
{
	if (!BIT_ISSET(mTypeMask, typeMask))
		return;

	auto component = mComponents.First();
	while (component != NULL) {
		auto next = component->ComponentLink.Tail;
		component->FireEvent(evt, typeMask);
		component = next;
	}

	SceneNode* child = mChildren.First();
	while (child != NULL) {
		auto next = child->SceneNodeLink.Tail;
		child->FireEvent(evt);
		child = next;
	}

	OnEventReceived(evt);
}

bool SceneNode::OnRegisterObject()
{
	mOnAddedToSceneGroupPtr = GetMethodPtr("OnAddedToSceneGroup");
	mOnRemovedFromSceneGroupPtr = GetMethodPtr("OnRemovedFromSceneGroup");
	mOnAttachedToSceneMethodPtr = GetMethodPtr("OnAttachedToScene");
	mOnDetachedFromSceneMethodPtr = GetMethodPtr("OnDetachedFromScene");
	mOnPositionChangedPtr = GetMethodPtr("OnPositionChanged");
	mOnRotationChangedPtr = GetMethodPtr("OnRotationChanged");
	mOnScaleChangedPtr = GetMethodPtr("OnScaleChanged");
	return true;
}

void SceneNode::OnEventReceived(const Event* evt)
{
}

void SceneNode::OnPositionChanged()
{
	mOnPositionChangedPtr->Invoke();
}

void SceneNode::OnRotationChanged()
{
	mOnRotationChangedPtr->Invoke();
}

void SceneNode::OnScaleChanged()
{
	mOnScaleChangedPtr->Invoke();
}
