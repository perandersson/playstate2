#include "../Memory.h"
#include "SceneGroup.h"
#include "SceneNode.h"
#include "Updatable.h"
#include "Tickable.h"
#include "Component.h"
#include "../script/ScriptUtils.h"
using namespace core;

SceneGroup::SceneGroup()
: ScriptObject(), mTypeMask(0),
mComponents(offsetof(Component, ComponentLink)),
mChildren(offsetof(SceneNode, SceneNodeLink))
{
}

SceneGroup::~SceneGroup()
{
	mChildren.DeleteAll();
	mComponents.DeleteAll();
}

void SceneGroup::AddSceneNode(SceneNode* node)
{
	assert_not_null(node);

	mChildren.AddLast(node);
	node->AddedToSceneGroup(this);
	OnSceneNodeAdded(node);
}

void SceneGroup::RemoveSceneNode(SceneNode* node)
{
	assert_not_null(node);
	assert(node->mSceneGroup == this && "You are not allowed to remove another scene objects child node");

	mChildren.Remove(node);
	node->RemovedFromSceneGroup(this);
	OnSceneNodeRemoved(node);
}

void SceneGroup::AddComponent(Component* component)
{
	assert_not_null(component);
	assert(component->GetSceneNode() == nullptr && "This component is already added to a scene node");
	assert(component->GetSceneGroup() == nullptr && "This component is already added to a scene group");

	//
	// Add the component to it's internal list
	//

	mComponents.AddLast(component);
	component->ComponentAddedToGroup(this);

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

	if (IsAttachedToScene())
		component->AttachedToScene();
}

void SceneGroup::RemoveComponent(Component* component)
{
	assert_not_null(component);
	assert(component->GetSceneNode() == nullptr && "You are not allowed to remove a component on that isn't attached this group");
	assert(component->GetSceneGroup() == this && "You are not allowed to remove a component on that isn't attached this group");

	auto updatable = component->GetUpdatable();
	if (updatable != nullptr) {
		RemoveUpdatable(updatable);
	}

	auto tickable = component->GetTickable();
	if (tickable != nullptr) {
		RemoveTickable(tickable);
	}

	mComponents.Remove(component);

	if (IsAttachedToScene())
		component->DetachedFromScene();

	component->ComponentRemovedFromGroup(this);
}

Component* SceneGroup::GetFirstComponent(typemask typeMask)
{
	auto component = mComponents.First();
	while (component != nullptr) {
		if (BIT_ISSET(component->GetTypeMask(), typeMask))
			return component;
		component = component->ComponentLink.Tail;
	}

	return nullptr;
}

void SceneGroup::AttachedToScene()
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

void SceneGroup::DetachedFromScene()
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

SceneNode* SceneGroup::FindSceneNode(const std::string& nodeID)
{
	auto node = mChildren.First();
	while (node != nullptr) {
		auto next = node->SceneNodeLink.Tail;
		auto found = node->FindSceneNode(nodeID);
		if (found != nullptr)
			return found;

		node = next;
	}

	return nullptr;
}

void SceneGroup::AddUpdatable(Updatable* updatable)
{
}

void SceneGroup::RemoveUpdatable(Updatable* updatable)
{
}

void SceneGroup::AddTickable(Tickable* tickable)
{
}

void SceneGroup::RemoveTickable(Tickable* tickable)
{
}

void SceneGroup::AddRenderable(Renderable* renderable)
{
}

void SceneGroup::RemoveRenderable(Renderable* renderable)
{
}

void SceneGroup::AddLightSource(LightSource* lightSource)
{
}

void SceneGroup::RemoveLightSource(LightSource* lightSource)
{
}

void SceneGroup::FireEvent(const Event* evt)
{
	auto child = mChildren.First();
	while (child != NULL) {
		auto  next = child->SceneNodeLink.Tail;
		child->FireEvent(evt);
		child = next;
	}
}

void SceneGroup::FireEvent(const Event* evt, typemask typeMask)
{
	auto child = mChildren.First();
	while (child != NULL) {
		auto  next = child->SceneNodeLink.Tail;
		child->FireEvent(evt, typeMask);
		child = next;
	}
}
void SceneGroup::SetGroupID(const std::string& groupID)
{
	mGroupID = groupID;
}

void SceneGroup::SetTypeMask(typemask typeMask)
{
	mTypeMask = typeMask;
}

void SceneGroup::SetPosition(const Vector3& position)
{
	mPosition = position;

	auto child = mChildren.First();
	while (child != NULL) {
		auto next = child->SceneNodeLink.Tail;
		child->UpdatePosition(Vector3::ZERO);
		child = next;
	}
}

void SceneGroup::UpdateUpdatables()
{
}

void SceneGroup::UpdateTickables()
{
}

bool SceneGroup::Find(const FindQuery& query, RenderBlockResultSet* _out_resultSet) const
{
	return false;
}

bool SceneGroup::Find(const FindQuery& query, LightSourceResultSet* _out_resultSet) const
{
	return false;
}

void SceneGroup::OnSceneNodeAdded(SceneNode* node)
{
}

void SceneGroup::OnSceneNodeRemoved(SceneNode* node)
{
}

void SceneGroup::OnAttachedToScene()
{
	mOnAttachedToSceneMethodPtr->Invoke();
}

void SceneGroup::OnDetachedFromScene()
{
	mOnDetachedFromSceneMethodPtr->Invoke();
}

bool SceneGroup::OnRegisterObject()
{
	mOnAttachedToSceneMethodPtr = GetMethodPtr("OnAttachedToScene");
	mOnDetachedFromSceneMethodPtr = GetMethodPtr("OnDetachedFromScene");
	return true;
}
