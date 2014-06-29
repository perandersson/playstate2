#include "../Memory.h"
#include "PointLight_script.h"
using namespace core;

int PointLight_Factory(struct lua_State* L)
{
	return ScriptUtils::PushObject(L, new PointLight());
}

int PointLight_Init(struct lua_State* L)
{
	if (!lua_istable(L, -1)) {
		lua_pop(L, 1);
		ScriptUtils::LogError(L, "Expected: PointLight.__init(self)");
		return 0;
	}

	ScriptUtils::SetObject(L, new PointLight());
	return 0;
}

int PointLight_SetColor(struct lua_State* L)
{
	Color value = ScriptUtils::ToColor(L);
	PointLight* self = ScriptUtils::ToObject<PointLight>(L);
	if (self != nullptr)
		self->SetColor(value);
	else
		ScriptUtils::LogError(L, "Expected: PointLight.SetColor(self, Color)");

	return 0;
}

int PointLight_SetRadius(struct lua_State* L)
{
	float32 value = ScriptUtils::ToFloat32(L);
	PointLight* self = ScriptUtils::ToObject<PointLight>(L);
	if (self != nullptr)
		self->SetRadius(value);
	else
		ScriptUtils::LogError(L, "Expected: PointLight.SetRadius(self, float32)");

	return 0;
}

int PointLight_SetConstantAttenuation(struct lua_State* L)
{
	float32 value = ScriptUtils::ToFloat32(L);
	PointLight* self = ScriptUtils::ToObject<PointLight>(L);
	if (self != nullptr)
		self->SetConstantAttenuation(value);
	else
		ScriptUtils::LogError(L, "Expected: PointLight.SetConstantAttenuation(self, float32)");

	return 0;
}

int PointLight_SetLinearAttenuation(struct lua_State* L)
{
	float32 value = ScriptUtils::ToFloat32(L);
	PointLight* self = ScriptUtils::ToObject<PointLight>(L);
	if (self != nullptr)
		self->SetLinearAttenuation(value);
	else
		ScriptUtils::LogError(L, "Expected: PointLight.SetLinearAttenuation(self, float32)");

	return 0;
}

int PointLight_SetQuadricAttenuation(struct lua_State* L)
{
	float32 value = ScriptUtils::ToFloat32(L);
	PointLight* self = ScriptUtils::ToObject<PointLight>(L);
	if (self != nullptr)
		self->SetQuadricAttenuation(value);
	else
		ScriptUtils::LogError(L, "Expected: PointLight.SetQuadricAttenuation(self, float32)");

	return 0;
}
