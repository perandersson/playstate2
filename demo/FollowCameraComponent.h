#pragma once
#include <core/playstate.h>
using namespace core;

class FollowCameraComponent : public Component, public Updatable
{
	DECLARE_SCRIPT_OBJECT(FollowCameraComponent);

public:
	FollowCameraComponent();
	virtual ~FollowCameraComponent();

	virtual Updatable* GetUpdatable() { return this; }

	// Updatable
public:
	virtual void Update();
};

int FollowCameraComponent_Factory(struct lua_State* L);
int FollowCameraComponent_Init(struct lua_State* L);

static luaL_Reg FollowCameraComponent_Methods[] = {
	{ SCRIPT_CONSTRUCTOR, FollowCameraComponent_Factory },
	{ SCRIPT_INHERIT_CONSTRUCTOR, FollowCameraComponent_Init },
	{ nullptr, nullptr }
};
