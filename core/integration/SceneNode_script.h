#pragma once
#include "../scene/SceneNode.h"
#include "../script/ScriptUtils.h"

int SceneNode_Factory(struct lua_State* L);
int SceneNode_Init(struct lua_State* L);
int SceneNode_SetTypeMask(struct lua_State* L);
int SceneNode_GetTypeMask(struct lua_State* L);
int SceneNode_SetID(struct lua_State* L);
int SceneNode_GetID(struct lua_State* L);
int SceneNode_AddChildNode(struct lua_State* L);
int SceneNode_RemoveChildNode(struct lua_State* L);
int SceneNode_AddComponent(struct lua_State* L);
int SceneNode_RemoveComponent(struct lua_State* L);
int SceneNode_SetPosition(struct lua_State* L);
int SceneNode_GetPosition(struct lua_State* L);
int SceneNode_GetAbsolutePosition(struct lua_State* L);
int SceneNode_SetRotation(struct lua_State* L);
int SceneNode_GetRotation(struct lua_State* L);
int SceneNode_GetAbsoluteRotation(struct lua_State* L);
int SceneNode_SetScale(struct lua_State* L);
int SceneNode_GetScale(struct lua_State* L);
int SceneNode_GetAbsoluteScale(struct lua_State* L);
int SceneNode_SetPositionRotationScale(struct lua_State* L);

static luaL_Reg SceneNode_Methods[] = {
	{ SCRIPT_CONSTRUCTOR, SceneNode_Factory },
	{ SCRIPT_INHERIT_CONSTRUCTOR, SceneNode_Init },
	{ "SetTypeMask", SceneNode_SetTypeMask },
	{ "GetTypeMask", SceneNode_GetTypeMask },
	{ "SetID", SceneNode_SetID },
	{ "GetID", SceneNode_GetID },
	{ "AddChildNode", SceneNode_AddChildNode },
	{ "RemoveChildNode", SceneNode_RemoveChildNode },
	{ "AddComponent", SceneNode_AddComponent },
	{ "RemoveComponent", SceneNode_RemoveComponent },
	{ "SetPosition", SceneNode_SetPosition },
	{ "GetPosition", SceneNode_GetPosition },
	{ "GetAbsolutePosition", SceneNode_GetAbsolutePosition },
	{ "SetRotation", SceneNode_SetRotation },
	{ "GetRotation", SceneNode_GetRotation },
	{ "GetAbsoluteRotation", SceneNode_GetAbsoluteRotation },
	{ "SetScale", SceneNode_SetScale },
	{ "GetScale", SceneNode_GetScale },
	{ "GetAbsoluteScale", SceneNode_GetAbsoluteScale },
	{ "SetPositionRotationScale", SceneNode_SetPositionRotationScale },
	{ nullptr, nullptr }
};
