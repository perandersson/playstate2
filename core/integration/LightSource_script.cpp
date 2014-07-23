#include "../Memory.h"
#include "LightSource_script.h"
using namespace core;

int LightSource_SetStaticShadowCaster(struct lua_State* L)
{
	bool b = ScriptUtils::ToBool(L);
	LightSource* self = ScriptUtils::ToObject<LightSource>(L);
	if (self != nullptr)
		self->SetStaticShadowCaster(b);
	else
		ScriptUtils::LogError(L, "Expected: LightSource.SetStaticShadowCaster(self, bool)");

	return 0;
}

int LightSource_GetStaticShadowCaster(struct lua_State* L)
{
	LightSource* self = ScriptUtils::ToObject<LightSource>(L);
	bool caster = false;
	if (self != nullptr)
		caster = self->GetStaticShadowCaster();
	else
		ScriptUtils::LogError(L, "Expected: LightSource.GetStaticShadowCaster(self)");

	return ScriptUtils::PushBool(L, caster);
}

int LightSource_SetDynamicShadowCaster(struct lua_State* L)
{
	bool b = ScriptUtils::ToBool(L);
	LightSource* self = ScriptUtils::ToObject<LightSource>(L);
	if (self != nullptr)
		self->SetDynamicShadowCaster(b);
	else
		ScriptUtils::LogError(L, "Expected: LightSource.SetDynamicShadowCaster(self, bool)");

	return 0;
}

int LightSource_GetDynamicShadowCaster(struct lua_State* L)
{
	LightSource* self = ScriptUtils::ToObject<LightSource>(L);
	bool caster = false;
	if (self != nullptr)
		caster = self->GetDynamicShadowCaster();
	else
		ScriptUtils::LogError(L, "Expected: LightSource.GetDynamicShadowCaster(self)");

	return ScriptUtils::PushBool(L, caster);
}
