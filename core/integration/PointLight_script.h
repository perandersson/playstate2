#pragma once
#include "../scene/light/PointLight.h"
#include "../script/ScriptUtils.h"

int PointLight_Factory(struct lua_State* L);
int PointLight_Init(struct lua_State* L);
int PointLight_SetColor(struct lua_State* L);
int PointLight_SetRadius(struct lua_State* L);
int PointLight_SetConstantAttenuation(struct lua_State* L);
int PointLight_SetLinearAttenuation(struct lua_State* L);
int PointLight_SetQuadricAttenuation(struct lua_State* L);

static luaL_Reg PointLight_Methods[] = {
	{ SCRIPT_CONSTRUCTOR, PointLight_Factory },
	{ SCRIPT_INHERIT_CONSTRUCTOR, PointLight_Init },
	{ "SetColor", PointLight_SetColor },
	{ "SetRadius", PointLight_SetRadius },
	{ "SetConstantAttenuation", PointLight_SetConstantAttenuation },
	{ "SetLinearAttenuation", PointLight_SetLinearAttenuation },
	{ "SetQuadricAttenuation", PointLight_SetQuadricAttenuation },
	{ nullptr, nullptr }
};
