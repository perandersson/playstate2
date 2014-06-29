#include "../Memory.h"
#include "LinkedListSceneGroup_script.h"
using namespace core;

int LinkedListSceneGroup_Factory(struct lua_State* L)
{
	return ScriptUtils::PushObject(L, new LinkedListSceneGroup());
}

int LinkedListSceneGroup_Init(struct lua_State* L)
{
	if (!lua_istable(L, -1)) {
		lua_pop(L, 1);
		ScriptUtils::LogError(L, "Expected: LinkedListSceneGroup.__init(self)");
		return 0;
	}

	ScriptUtils::SetObject(L, new LinkedListSceneGroup());
	return 0;
}
