#include "../Memory.h"
#include "SoundEngine_script.h"
#include "SoundReceiver_script.h"
using namespace core;

int SoundEngine_GetSoundReceiver(struct lua_State* L)
{
	const uint32 idx = ScriptUtils::ToUInt32(L);
	ISoundReceiver* soundReceiver = SoundEngine::GetSoundReceiver(idx);
	if (soundReceiver == nullptr) {
		lua_pushnil(L);
		return 1;
	}

	return ScriptUtils::PushObject(L, new ScriptSoundReceiver(soundReceiver));
}
