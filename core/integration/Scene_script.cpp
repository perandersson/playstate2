#include "../Memory.h"
#include "Scene_script.h"
#include "../kernel/Kernel.h"
using namespace core;

int Scene_GetActiveCamera(struct lua_State* L)
{
	Scene* scene = Kernel::GetGame()->GetScene();
	return ScriptUtils::PushObject(L, scene->GetActiveCamera());
}

int Scene_SetActiveCamera(struct lua_State* L)
{
	Camera* camera = ScriptUtils::ToObject<Camera>(L);
	if (camera != nullptr) {
		Kernel::GetGame()->GetScene()->SetActiveCamera(camera);
	}
	else {
		ScriptUtils::LogError(L, "Expected: Scene.SetActiveCamera(camera)");
	}
	return 0;
}

int Scene_SetAmbientLight(struct lua_State* L)
{
	const Color ambientColor = ScriptUtils::ToColor(L);
	Kernel::GetGame()->GetScene()->SetAmbientLight(ambientColor);
	return 0;
}

int Scene_GetAmbientLight(struct lua_State* L)
{
	const Color& ambientColor = Kernel::GetGame()->GetScene()->GetAmbientLight();
	return ScriptUtils::PushColor(L, ambientColor);
}
