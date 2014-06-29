#include "../Memory.h"
#include "SceneGroup_script.h"
#include "SceneNode_script.h"
using namespace core;

int SceneGroup_Factory(struct lua_State* L)
{
	return ScriptUtils::PushObject(L, new SceneGroup());
}

int SceneGroup_Init(struct lua_State* L)
{
	if (!lua_istable(L, -1)) {
		lua_pop(L, 1);
		ScriptUtils::LogError(L, "Expected: SceneGroup.__init(self)");
		return 0;
	}

	ScriptUtils::SetObject(L, new SceneGroup());
	return 0;
}

int SceneGroup_AddSceneNode(struct lua_State* L)
{
	SceneNode* node = ScriptUtils::ToObject<SceneNode>(L);
	SceneGroup* self = ScriptUtils::ToObject<SceneGroup>(L);
	if (node != nullptr && self != nullptr)
		self->AddSceneNode(node);

	return 0;
}

int SceneGroup_RemoveSceneNode(struct lua_State* L)
{
	SceneNode* node = ScriptUtils::ToObject<SceneNode>(L);
	SceneGroup* self = ScriptUtils::ToObject<SceneGroup>(L);
	if (node != nullptr && self != nullptr)
		self->RemoveSceneNode(node);

	return 0;
}

int SceneGroup_SetPosition(struct lua_State* L)
{
	Vector3 position = ScriptUtils::ToVector3(L);
	SceneGroup* self = ScriptUtils::ToObject<SceneGroup>(L);
	if (self != nullptr)
		self->SetPosition(position);
	else {
		ScriptUtils::LogError(L, "Expected: SceneGroup.SetPosition(self, Vector3)");
	}
	return 0;
}

