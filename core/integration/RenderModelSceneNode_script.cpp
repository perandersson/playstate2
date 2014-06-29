#include "../Memory.h"
#include "RenderModelSceneNode_script.h"
using namespace core;

int RenderModelSceneNode_Factory(struct lua_State* L)
{
	return ScriptUtils::PushObject(L, new RenderModelSceneNode());
}

int RenderModelSceneNode_Init(struct lua_State* L)
{
	if (!lua_istable(L, -1)) {
		lua_pop(L, 1);
		ScriptUtils::LogError(L, "Expected: RenderModelSceneNode.__init(self)");
		return 0;
	}

	ScriptUtils::SetObject(L, new RenderModelSceneNode());
	return 0;
}

int RenderModelSceneNode_SetModel(struct lua_State* L)
{
	Resource<Model> model = ScriptUtils::ToResource<Model>(L);
	RenderModelSceneNode* self = ScriptUtils::ToObject<RenderModelSceneNode>(L);
	if (self != nullptr)
		self->SetModel(model);
	else
		ScriptUtils::LogError(L, "Expected: RenderModelSceneNode.SetModel(self, Resource<Model>)");

	return 0;
}
