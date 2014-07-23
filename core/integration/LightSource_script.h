#pragma once
#include "../scene/light/LightSource.h"
#include "../script/ScriptUtils.h"

int LightSource_SetStaticShadowCaster(struct lua_State* L);
int LightSource_GetStaticShadowCaster(struct lua_State* L);
int LightSource_SetDynamicShadowCaster(struct lua_State* L);
int LightSource_GetDynamicShadowCaster(struct lua_State* L);


static luaL_Reg LightSource_Methods[] = {
	{ "SetStaticShadowCaster", LightSource_SetStaticShadowCaster },
	{ "GetStaticShadowCaster", LightSource_GetStaticShadowCaster },
	{ "SetDynamicShadowCaster", LightSource_SetDynamicShadowCaster },
	{ "GetDynamicShadowCaster", LightSource_GetDynamicShadowCaster },
	{ nullptr, nullptr }
};
