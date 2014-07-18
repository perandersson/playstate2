#include "../Memory.h"
#include "SpotLight_script.h"
using namespace core;

int SpotLight_Factory(struct lua_State* L)
{
	return ScriptUtils::PushObject(L, new SpotLight());
}

int SpotLight_Init(struct lua_State* L)
{
	if (!lua_istable(L, -1)) {
		lua_pop(L, 1);
		ScriptUtils::LogError(L, "Expected: SpotLight.__init(self)");
		return 0;
	}

	ScriptUtils::SetObject(L, new SpotLight());
	return 0;
}

int SpotLight_SetColor(struct lua_State* L)
{
	Color value = ScriptUtils::ToColor(L);
	SpotLight* self = ScriptUtils::ToObject<SpotLight>(L);
	if (self != nullptr)
		self->SetColor(value);
	else
		ScriptUtils::LogError(L, "Expected: SpotLight.SetColor(self, Color)");

	return 0;
}

int SpotLight_GetColor(struct lua_State* L)
{
	SpotLight* self = ScriptUtils::ToObject<SpotLight>(L);
	if (self != nullptr)
		return ScriptUtils::PushColor(L, self->GetColor());
	else
		ScriptUtils::LogError(L, "Expected: SpotLight.GetColor(self)");

	return ScriptUtils::PushColor(L, Color::NOTHING);
}

int SpotLight_SetCutoff(struct lua_State* L)
{
	float32 value = ScriptUtils::ToFloat32(L);
	SpotLight* self = ScriptUtils::ToObject<SpotLight>(L);
	if (self != nullptr)
		self->SetCutoff(value);
	else
		ScriptUtils::LogError(L, "Expected: SpotLight.SetCutoff(self, float32)");

	return 0;
}

int SpotLight_GetCutoff(struct lua_State* L)
{
	SpotLight* self = ScriptUtils::ToObject<SpotLight>(L);
	if (self != nullptr)
		return ScriptUtils::PushFloat32(L, self->GetCutoff());
	else
		ScriptUtils::LogError(L, "Expected: SpotLight.GetCutoff(self)");

	return ScriptUtils::PushFloat32(L, 0.0f);
}

int SpotLight_SetSpotDirection(struct lua_State* L)
{
	Vector3 value = ScriptUtils::ToVector3(L);
	SpotLight* self = ScriptUtils::ToObject<SpotLight>(L);
	if (self != nullptr)
		self->SetSpotDirection(value);
	else
		ScriptUtils::LogError(L, "Expected: SpotLight.SetSpotDirection(self, Vector3)");

	return 0;
}

int SpotLight_GetSpotDirection(struct lua_State* L)
{
	SpotLight* self = ScriptUtils::ToObject<SpotLight>(L);
	if (self != nullptr)
		return ScriptUtils::PushVector3(L, self->GetSpotDirection());
	else
		ScriptUtils::LogError(L, "Expected: SpotLight.GetSpotDirection(self)");

	return ScriptUtils::PushVector3(L, Vector3::ZERO);
}

int SpotLight_SetTexture(struct lua_State* L)
{
	Resource<Texture2D> value = ScriptUtils::ToResource<Texture2D>(L);
	SpotLight* self = ScriptUtils::ToObject<SpotLight>(L);
	if (self != nullptr)
		self->SetTexture(value);
	else
		ScriptUtils::LogError(L, "Expected: SpotLight.SetTexture(self, Texture2D)");

	return 0;
}

int SpotLight_SetSpotExponent(struct lua_State* L)
{
	float32 value = ScriptUtils::ToFloat32(L);
	SpotLight* self = ScriptUtils::ToObject<SpotLight>(L);
	if (self != nullptr)
		self->SetSpotExponent(value);
	else
		ScriptUtils::LogError(L, "Expected: SpotLight.SetSpotExponent(self, float32)");

	return 0;
}

int SpotLight_GetSpotExponent(struct lua_State* L)
{
	SpotLight* self = ScriptUtils::ToObject<SpotLight>(L);
	if (self != nullptr)
		return ScriptUtils::PushFloat32(L, self->GetSpotExponent());
	else
		ScriptUtils::LogError(L, "Expected: SpotLight.GetSpotExponent(self)");

	return ScriptUtils::PushFloat32(L, 0.0f);
}
