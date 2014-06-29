#include "../Memory.h"
#include "QuadTreeSceneGroup_script.h"
using namespace core;

int QuadTreeSceneGroup_Factory(struct lua_State* L)
{
	return ScriptUtils::PushObject(L, new QuadTreeSceneGroup());
}

int QuadTreeSceneGroup_Init(struct lua_State* L)
{
	if (!lua_istable(L, -1)) {
		lua_pop(L, 1);
		ScriptUtils::LogError(L, "Expected: QuadTreeSceneGroup.__init(self)");
		return 0;
	}

	ScriptUtils::SetObject(L, new QuadTreeSceneGroup());
	return 0;
}

int QuadTreeSceneGroup_SetQuadTree(struct lua_State* L)
{
	uint32 maxDepth = ScriptUtils::ToUInt32(L);
	float32 depth = ScriptUtils::ToFloat32(L);
	float32 height = ScriptUtils::ToFloat32(L);
	float32 width = ScriptUtils::ToFloat32(L);
	Vector3 position = ScriptUtils::ToVector3(L);

	QuadTreeSceneGroup* self = ScriptUtils::ToObject<QuadTreeSceneGroup>(L);
	if (self != nullptr)
		self->SetQuadTree(position, width, height, depth, maxDepth);
	else
		ScriptUtils::LogError(L, "Expected: QuadTreeSceneGroup.SetQuadTree(self, Vector3, float32, float32, float32, uint32)");
	return 0;
}
