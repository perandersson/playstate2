#include "../Memory.h"
#include "SoundEffect_script.h"
using namespace core;

//
// Script integration
//

int SoundEffect_Play(struct lua_State* L)
{
	Resource<SoundEffect> effect = ScriptUtils::ToResource<SoundEffect>(L);
	if (effect.IsNull()) {
		ScriptUtils::LogError(L, "Expected: SoundEffect.Play(Resource<SoundEffect>)");
		return 0;
	}
	
	SoundEngine::Play(effect);
	return 0;
}

int SoundEffect_GetDuration(struct lua_State* L)
{
	Resource<SoundEffect> effect = ScriptUtils::ToResource<SoundEffect>(L);
	if (effect.IsNull()) {
		ScriptUtils::LogError(L, "Expected: SoundEffect.Play(Resource<SoundEffect>)");
		return ScriptUtils::PushFloat32(L, 0.0f);
	}

	return ScriptUtils::PushFloat32(L, effect->GetDuration());
}
