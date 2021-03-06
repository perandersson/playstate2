#pragma once
#include "../scene/light/SpotLight.h"
#include "../script/ScriptUtils.h"

int SpotLight_Factory(struct lua_State* L);
int SpotLight_Init(struct lua_State* L);
int SpotLight_SetColor(struct lua_State* L);
int SpotLight_GetColor(struct lua_State* L);
int SpotLight_SetCutoff(struct lua_State* L);
int SpotLight_GetCutoff(struct lua_State* L);
int SpotLight_SetSpotDirection(struct lua_State* L);
int SpotLight_GetSpotDirection(struct lua_State* L);
int SpotLight_SetTexture(struct lua_State* L);
int SpotLight_SetSpotExponent(struct lua_State* L);
int SpotLight_GetSpotExponent(struct lua_State* L);

static luaL_Reg SpotLight_Methods[] = {
	{ SCRIPT_CONSTRUCTOR, SpotLight_Factory },
	{ SCRIPT_INHERIT_CONSTRUCTOR, SpotLight_Init },
	{ "SetColor", SpotLight_SetColor },
	{ "GetColor", SpotLight_GetColor },
	{ "SetCutoff", SpotLight_SetCutoff },
	{ "GetCutoff", SpotLight_GetCutoff },
	{ "SetSpotDirection", SpotLight_SetSpotDirection },
	{ "GetSpotDirection", SpotLight_GetSpotDirection },
	{ "SetTexture", SpotLight_SetTexture },
	{ "SetSpotExponent", SpotLight_SetSpotExponent },
	{ "GetSpotExponent", SpotLight_GetSpotExponent },
	{ nullptr, nullptr }
};
