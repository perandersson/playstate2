#include "../Memory.h"
#include "Camera_script.h"
using namespace core;

int Camera_Move(struct lua_State* L)
{
	const Vector3 direction = ScriptUtils::ToVector3(L);
	Camera* camera = ScriptUtils::ToObject<Camera>(L);
	if (camera != nullptr)
		camera->Move(direction);
	else
		ScriptUtils::LogError(L, "Expected: Camera.Move(self, Vector3)");

	return 0;
}

int Camera_LookAt(struct lua_State* L)
{
	const Vector3 up = ScriptUtils::ToVector3(L);
	const Vector3 center = ScriptUtils::ToVector3(L);
	const Vector3 eye = ScriptUtils::ToVector3(L);
	Camera* camera = ScriptUtils::ToObject<Camera>(L);
	if (camera != nullptr)
		camera->LookAt(eye, center, up);
	else
		ScriptUtils::LogError(L, "Expected: Camera.LookAt(self, Vector3, Vector3, Vector3)");

	return 0;
}

int Camera_GetPosition(struct lua_State* L)
{
	Camera* camera = ScriptUtils::ToObject<Camera>(L);
	if (camera != nullptr)
		return ScriptUtils::PushVector3(L, camera->GetPosition());
	else
		ScriptUtils::LogError(L, "Expected: Camera.GetPosition(self)");

	return ScriptUtils::PushVector3(L, Vector3::ZERO);
}

int Camera_GetUp(struct lua_State* L)
{
	Camera* camera = ScriptUtils::ToObject<Camera>(L);
	if (camera != nullptr)
		return ScriptUtils::PushVector3(L, camera->GetUp());
	else
		ScriptUtils::LogError(L, "Expected: Camera.GetUp(self)");

	return ScriptUtils::PushVector3(L, Vector3::ZERO);
}

int Camera_GetCenter(struct lua_State* L)
{
	Camera* camera = ScriptUtils::ToObject<Camera>(L);
	if (camera != nullptr)
		return ScriptUtils::PushVector3(L, camera->GetCenter());
	else
		ScriptUtils::LogError(L, "Expected: Camera.GetCenter(self)");

	return ScriptUtils::PushVector3(L, Vector3::ZERO);
}
