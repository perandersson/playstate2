#include "../Memory.h"
#include "SceneNode_script.h"
#include "Component_script.h"
using namespace core;

int SceneNode_Factory(struct lua_State* L)
{
	return ScriptUtils::PushObject(L, new SceneNode());
}

int SceneNode_Init(struct lua_State* L)
{
	if (!lua_istable(L, -1)) {
		lua_pop(L, 1);
		ScriptUtils::LogError(L, "Expected: SceneNode.__init(self)");
		return 0;
	}

	ScriptUtils::SetObject(L, new SceneNode());
	return 0;
}

int SceneNode_SetTypeMask(struct lua_State* L)
{
	typemask typeMask = ScriptUtils::ToTypeMask(L);
	SceneNode* self = ScriptUtils::ToObject<SceneNode>(L);
	if (self != nullptr)
		self->SetTypeMask(typeMask);
	else
		ScriptUtils::LogError(L, "Expected: SceneNode.SetTypeMask(self, typemask)");

	return 0;
}

int SceneNode_GetTypeMask(struct lua_State* L)
{
	SceneNode* self = ScriptUtils::ToObject<SceneNode>(L);
	typemask value = 0;
	if (self != nullptr)
		value = self->GetTypeMask();
	else
		ScriptUtils::LogError(L, "Expected: SceneNode.GetTypeMask(self)");

	return ScriptUtils::PushTypeMask(L, value);
}

int SceneNode_SetID(struct lua_State* L)
{
	std::string name = ScriptUtils::ToString(L);
	SceneNode* self = ScriptUtils::ToObject<SceneNode>(L);
	if (self != nullptr)
		self->SetID(name);
	else
		ScriptUtils::LogError(L, "Expected: SceneNode.SetID(self, string)");

	return 0;
}

int SceneNode_GetID(struct lua_State* L)
{
	std::string name;
	SceneNode* self = ScriptUtils::ToObject<SceneNode>(L);
	if (self != nullptr)
		name = self->GetID();
	else
		ScriptUtils::LogError(L, "Expected: SceneNode.GetID(self)");

	return ScriptUtils::PushString(L, name);
}

int SceneNode_AddChildNode(struct lua_State* L)
{
	SceneNode* child = ScriptUtils::ToObject<SceneNode>(L);
	SceneNode* self = ScriptUtils::ToObject<SceneNode>(L);
	if (child != nullptr && self != nullptr)
		self->AddChildNode(child);
	else
		ScriptUtils::LogError(L, "Expected: SceneNode.AddChildNode(self, SceneNode)");

	return 0;
}

int SceneNode_RemoveChildNode(struct lua_State* L)
{
	SceneNode* child = ScriptUtils::ToObject<SceneNode>(L);
	SceneNode* self = ScriptUtils::ToObject<SceneNode>(L);
	if (child != nullptr && self != nullptr)
		self->RemoveChildNode(child);
	else
		ScriptUtils::LogError(L, "Expected: SceneNode.RemoveChildNode(self, SceneNode)");

	return 0;
}

int SceneNode_AddComponent(struct lua_State* L)
{
	Component* component = ScriptUtils::ToObject<Component>(L);
	SceneNode* self = ScriptUtils::ToObject<SceneNode>(L);
	if (component != nullptr && self != nullptr)
		self->AddComponent(component);
	else
		ScriptUtils::LogError(L, "Expected: SceneNode.AddComponent(self, Component)");

	return 0;
}

int SceneNode_RemoveComponent(struct lua_State* L)
{
	Component* component = ScriptUtils::ToObject<Component>(L);
	SceneNode* self = ScriptUtils::ToObject<SceneNode>(L);
	if (component != nullptr && self != nullptr)
		self->RemoveComponent(component);
	else
		ScriptUtils::LogError(L, "Expected: SceneNode.RemoveComponent(self, Component)");

	return 0;
}

int SceneNode_SetPosition(struct lua_State* L)
{
	Vector3 position = ScriptUtils::ToVector3(L);
	SceneNode* self = ScriptUtils::ToObject<SceneNode>(L);
	if (self != nullptr)
		self->SetPosition(position);
	else
		ScriptUtils::LogError(L, "Expected: SceneNode.SetPosition(self, Vector3)");

	return 0;
}

int SceneNode_GetPosition(struct lua_State* L)
{
	Vector3 result;
	SceneNode* self = ScriptUtils::ToObject<SceneNode>(L);
	if (self != nullptr)
		result = self->GetPosition();
	else
		ScriptUtils::LogError(L, "Expected: SceneNode.GetPosition(self)");

	return ScriptUtils::PushVector3(L, result);
}

int SceneNode_GetAbsolutePosition(struct lua_State* L)
{
	Vector3 result;
	SceneNode* self = ScriptUtils::ToObject<SceneNode>(L);
	if (self != nullptr)
		result = self->GetAbsolutePosition();
	else
		ScriptUtils::LogError(L, "Expected: SceneNode.GetAbsolutePosition(self)");

	return ScriptUtils::PushVector3(L, result);
}

int SceneNode_SetRotation(struct lua_State* L)
{
	Vector3 rotation = ScriptUtils::ToVector3(L);
	SceneNode* self = ScriptUtils::ToObject<SceneNode>(L);
	if (self != nullptr)
		self->SetRotation(rotation);
	else
		ScriptUtils::LogError(L, "Expected: SceneNode.SetRotation(self, Vector3)");

	return 0;
}

int SceneNode_GetRotation(struct lua_State* L)
{
	Vector3 result;
	SceneNode* self = ScriptUtils::ToObject<SceneNode>(L);
	if (self != nullptr)
		result = self->GetRotation();
	else
		ScriptUtils::LogError(L, "Expected: SceneNode.GetRotation(self)");

	return ScriptUtils::PushVector3(L, result);
}

int SceneNode_GetAbsoluteRotation(struct lua_State* L)
{
	Vector3 result;
	SceneNode* self = ScriptUtils::ToObject<SceneNode>(L);
	if (self != nullptr)
		result = self->GetAbsoluteRotation();
	else
		ScriptUtils::LogError(L, "Expected: SceneNode.GetAbsoluteRotation(self)");

	return ScriptUtils::PushVector3(L, result);
}

int SceneNode_SetScale(struct lua_State* L)
{
	Vector3 scale = ScriptUtils::ToVector3(L);
	SceneNode* self = ScriptUtils::ToObject<SceneNode>(L);
	if (self != nullptr)
		self->SetScale(scale);
	else
		ScriptUtils::LogError(L, "Expected: SceneNode.SetScale(self, Vector3)");

	return 0;
}

int SceneNode_GetScale(struct lua_State* L)
{
	Vector3 result;
	SceneNode* self = ScriptUtils::ToObject<SceneNode>(L);
	if (self != nullptr)
		result = self->GetScale();
	else
		ScriptUtils::LogError(L, "Expected: SceneNode.GetScale(self)");

	return ScriptUtils::PushVector3(L, result);
}

int SceneNode_GetAbsoluteScale(struct lua_State* L)
{
	Vector3 result;
	SceneNode* self = ScriptUtils::ToObject<SceneNode>(L);
	if (self != nullptr)
		result = self->GetAbsoluteScale();
	else
		ScriptUtils::LogError(L, "Expected: SceneNode.GetAbsoluteScale(self)");

	return ScriptUtils::PushVector3(L, result);
}

int SceneNode_SetPositionRotationScale(struct lua_State* L)
{
	Vector3 scale = ScriptUtils::ToVector3(L);
	Vector3 rotation = ScriptUtils::ToVector3(L);
	Vector3 position = ScriptUtils::ToVector3(L);
	SceneNode* self = ScriptUtils::ToObject<SceneNode>(L);
	if (self != nullptr) {
		self->SetPosition(position);
		self->SetRotation(rotation);
		self->SetScale(scale);
	}
	else
		ScriptUtils::LogError(L, "Expected: SceneNode.SetPositionRotationScale(self, Vector3, Vector3, Vector3)");

	return 0;
}
