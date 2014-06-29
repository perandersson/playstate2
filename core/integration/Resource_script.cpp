#include "../Memory.h"
#include "Resource_script.h"
using namespace core;

int Resource_Load(struct lua_State* L)
{
	std::string path = ScriptUtils::ToString(L);
	Resource<ResourceObject> resource = ResourceManager::GetResource<ResourceObject>(path);
	ScriptUtils::PushResource(L, resource);
	return 1;
}
