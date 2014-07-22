#pragma once
#include "../scene/rendering/Renderable.h"
#include "../script/ScriptUtils.h"

int Renderable_SetStaticShadowCaster(struct lua_State* L);
int Renderable_GetStaticShadowCaster(struct lua_State* L);
int Renderable_SetDynamicShadowCaster(struct lua_State* L);
int Renderable_GetDynamicShadowCaster(struct lua_State* L);

static luaL_Reg Renderable_Methods[] = {
	{ "SetStaticShadowCaster", Renderable_SetStaticShadowCaster },
	{ "GetStaticShadowCaster", Renderable_GetStaticShadowCaster },
	{ "SetDynamicShadowCaster", Renderable_SetDynamicShadowCaster },
	{ "GetDynamicShadowCaster", Renderable_GetDynamicShadowCaster },
	{ nullptr, nullptr }
};
