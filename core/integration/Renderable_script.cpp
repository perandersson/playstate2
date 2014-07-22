#include "../Memory.h"
#include "Renderable_script.h"
using namespace core;

int Renderable_SetStaticShadowCaster(struct lua_State* L)
{
	bool b = ScriptUtils::ToBool(L);
	Renderable* self = ScriptUtils::ToObject<Renderable>(L);
	if (self != nullptr)
		self->SetStaticShadowCaster(b);
	else
		ScriptUtils::LogError(L, "Expected: Renderable.SetStaticShadowCaster(self, bool)");

	return 0;
}

int Renderable_GetStaticShadowCaster(struct lua_State* L)
{
	Renderable* self = ScriptUtils::ToObject<Renderable>(L);
	bool caster = false;
	if (self != nullptr)
		caster = self->GetStaticShadowCaster();
	else
		ScriptUtils::LogError(L, "Expected: Renderable.GetStaticShadowCaster(self)");

	return ScriptUtils::PushBool(L, caster);
}

int Renderable_SetDynamicShadowCaster(struct lua_State* L)
{
	bool b = ScriptUtils::ToBool(L);
	Renderable* self = ScriptUtils::ToObject<Renderable>(L);
	if (self != nullptr)
		self->SetDynamicShadowCaster(b);
	else
		ScriptUtils::LogError(L, "Expected: Renderable.SetDynamicShadowCaster(self, bool)");

	return 0;
}

int Renderable_GetDynamicShadowCaster(struct lua_State* L)
{
	Renderable* self = ScriptUtils::ToObject<Renderable>(L);
	bool caster = false;
	if (self != nullptr)
		caster = self->GetDynamicShadowCaster();
	else
		ScriptUtils::LogError(L, "Expected: Renderable.GetDynamicShadowCaster(self)");

	return ScriptUtils::PushBool(L, caster);
}
