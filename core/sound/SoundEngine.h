#pragma once
#include "ISoundEngine.h"

namespace core
{
	class SoundEngine
	{
	public:
		static void Play(const Resource<SoundEffect> effect);
		static void Play(const Resource<SoundEffect> effect, const Vector3& position);
		static void Play(const SoundEffect* effect);
		static void Play(const SoundEffect* effect, const Vector3& position);
		static void Play(Resource<Music> music);
		static void Play(Resource<Music> music, float32 fadeInTime);
		static void Stop(Resource<Music> music);
		static void Stop(Resource<Music> music, float32 fadeOutTime);
		static void Play(Music* music);
		static void Play(Music* music, float32 fadeInTime);
		static void Stop(Music* music);
		static void Stop(Music* music, float32 fadeOutTime);
		static ISoundReceiver* GetSoundReceiver(uint32 index);
	};
}
