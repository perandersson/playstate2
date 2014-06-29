#include "../Memory.h"
#include "Component_script.h"
#include "../scene/SceneGroup.h"
using namespace core;

ScriptableComponent::ScriptableComponent()
: Component(), Updatable(), Tickable()
{

}

ScriptableComponent::~ScriptableComponent()
{

}

void ScriptableComponent::OnComponentAdded()
{
}

Updatable* ScriptableComponent::GetUpdatable()
{
	return mUpdateMethodPtr.get() != nullptr ? this : nullptr;
}

Tickable* ScriptableComponent::GetTickable()
{
	return mTickMethodPtr.get() != nullptr ? this : nullptr;
}

bool ScriptableComponent::OnRegisterObject()
{
	mUpdateMethodPtr = GetMethodPtr("Update");
	mTickMethodPtr = GetMethodPtr("Tick");
	mOnAttachedToSceneMethodPtr = GetMethodPtr("OnAttachedToScene");
	mOnDetachedFromSceneMethodPtr = GetMethodPtr("OnDetachedFromScene");
	return true;
}

void ScriptableComponent::Update()
{
	mUpdateMethodPtr->Invoke();
}

void ScriptableComponent::Tick()
{
	mTickMethodPtr->Invoke();
}

void ScriptableComponent::OnAttachedToScene()
{
	mOnAttachedToSceneMethodPtr->Invoke();
}

void ScriptableComponent::OnDetachedFromScene()
{
	mOnDetachedFromSceneMethodPtr->Invoke();
}

int Component_Factory(struct lua_State* L)
{
	return ScriptUtils::PushObject(L, new ScriptableComponent());
}

int Component_Init(struct lua_State* L)
{
	if (!lua_istable(L, -1)) {
		lua_pop(L, 1);
		ScriptUtils::LogError(L, "Expected: Component.__init(self)");
		return 0;
	}

	ScriptUtils::SetObject(L, new ScriptableComponent());
	return 0;
}

int Component_GetSceneNode(struct lua_State* L)
{
	Component* self = ScriptUtils::ToObject<Component>(L);
	SceneNode* node = nullptr;
	if (self != nullptr)
		node = self->GetSceneNode();
	else
		ScriptUtils::LogError(L, "Expected: Component.GetSceneNode(self)");

	return ScriptUtils::PushObject(L, node);
}

int Component_GetSceneGroup(struct lua_State* L)
{
	Component* self = ScriptUtils::ToObject<Component>(L);
	SceneGroup* group = nullptr;
	if (self != nullptr) {
		auto node = self->GetSceneNode();
		if (node != nullptr)
			group = node->GetGroup();
	}
	else
		ScriptUtils::LogError(L, "Expected: Component.GetSceneGroup(self)");

	return ScriptUtils::PushObject(L, group);
}
