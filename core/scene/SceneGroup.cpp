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
	assert_not_implemented();
}

void SceneGroup::RemoveComponent(Component* component)
{
	assert_not_implemented();
}

Component* SceneGroup::GetFirstComponent(typemask typeMask)
{
	assert_not_implemented();
	return nullptr;
}

void SceneGroup::AttachedToScene()
{
	this->OnAttachedToScene();

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
