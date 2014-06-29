#include <core/Memory.h>
#include "FollowCameraComponent.h"

FollowCameraComponent::FollowCameraComponent()
: Component()
{
}

FollowCameraComponent::~FollowCameraComponent()
{
}

void FollowCameraComponent::Update()
{
	Camera* camera = Game::GetScene()->GetActiveCamera();
	const Vector3& nodePos = GetSceneNode()->GetAbsolutePosition();
	Vector3 eye = Vector3(0, 50.0f, 50.0f) + nodePos;

	camera->LookAt(eye, nodePos, camera->GetUp());
}

int FollowCameraComponent_Factory(struct lua_State* L)
{
	return ScriptUtils::PushObject(L, new FollowCameraComponent());
}

int FollowCameraComponent_Init(struct lua_State* L)
{
	if (!lua_istable(L, -1)) {
		lua_pop(L, 1);
		ScriptUtils::LogError(L, "Expected: FollowCameraComponent.__init(self)");
		return 0;
	}

	ScriptUtils::SetObject(L, new FollowCameraComponent());
	return 0;
}
