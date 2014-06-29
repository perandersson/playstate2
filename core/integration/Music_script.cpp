#include "../Memory.h"
#include "Music_script.h"
using namespace core;

int Music_Play(struct lua_State* L)
{
	Resource<Music> music = ScriptUtils::ToResource<Music>(L);
	if (music.IsNull()) {
		ScriptUtils::LogError(L, "Expected: Music.Play(Resource<Music>)");
		return 0;
	}

	SoundEngine::Play(music);
	return 0;
}

int Music_FadeInPlay(struct lua_State* L)
{
	float32 fadeInTime = ScriptUtils::ToFloat32(L);
	Resource<Music> music = ScriptUtils::ToResource<Music>(L);
	if (music.IsNull()) {
		ScriptUtils::LogError(L, "Expected: Music.FadeInPlay(Resource<Music>, float32)");
		return 0;
	}

	SoundEngine::Play(music, fadeInTime);
	return 0;
}

int Music_Stop(struct lua_State* L)
{
	Resource<Music> music = ScriptUtils::ToResource<Music>(L);
	if (music.IsNull()) {
		ScriptUtils::LogError(L, "Expected: Music.Stop(Resource<Music>)");
		return 0;
	}

	SoundEngine::Stop(music);
	return 0;
}

int Music_FadeOutStop(struct lua_State* L)
{
	float32 fadeOutTime = ScriptUtils::ToFloat32(L);
	Resource<Music> music = ScriptUtils::ToResource<Music>(L);
	if (music.IsNull()) {
		ScriptUtils::LogError(L, "Expected: Music.FadeOutStop(Resource<Music>, float32)");
		return 0;
	}

	SoundEngine::Stop(music, fadeOutTime);
	return 0;
}

