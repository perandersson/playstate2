#pragma once
#include "../ISoundEngine.h"
#include "../../LinkedList.h"
#include "OpenALMusic.h"
#include "OpenALSoundEffect.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

namespace core
{
	class IConfiguration;

	// The maximum number of sources the engine tries to allocate
	static const uint32 MaxMusicSources = 3U;
	static const uint32 MaxSoundSources = 29U;
	static const uint32 MaxSources = MaxMusicSources + MaxSoundSources;

	class OpenALSoundEngine : public ISoundEngine, public ISoundReceiver
	{
	public:
		OpenALSoundEngine(IConfiguration* configuration);
		virtual ~OpenALSoundEngine();

		/*!
			\brief Update the internal buffers used for streaming media.
		*/
		void UpdateStreamingBuffers();

	// ISoundEngine
	public:
		virtual void Play(const Resource<SoundEffect> effect);
		virtual void Play(const Resource<SoundEffect> effect, const Vector3& position);
		virtual void Play(const SoundEffect* effect);
		virtual void Play(const SoundEffect* effect, const Vector3& position);
		virtual void Play(Resource<Music> music);
		virtual void Play(Resource<Music> music, float32 fadeInTime);
		virtual void Stop(Resource<Music> music);
		virtual void Stop(Resource<Music> music, float32 fadeOutTime);
		virtual void Play(Music* music);
		virtual void Play(Music* music, float32 fadeInTime);
		virtual void Stop(Music* music);
		virtual void Stop(Music* music, float32 fadeOutTime);
		virtual void SetMasterVolume(float32 volume);
		virtual void SetMusicVolume(float32 volume);
		virtual void SetSoundEffectVolume(float32 volume);
		virtual ISoundReceiver* GetSoundReceiver(uint32 index);

	// ISoundListener
	public:
		virtual void SetPosition(const Vector3& position);
		virtual void LookAt(const Vector3& direction, const Vector3& up);

	private:
		//
		// @return The next source in the sources round-robin list. 
		ALuint FindNextSource();

		ALuint FindNextMusicSource();

		void PlaySoundEffectBuffer(ALuint sourceID, ALuint bufferID);

	private:
		ALuint mMusicSources[MaxMusicSources];
		uint32 mNextMusicSourceIndex;
		LinkedList<OpenALMusic> mMusicResources;

		ALuint mSources[MaxSoundSources];
		uint32 mNumSources;
		uint32 mNextSourceIndex;

		float32 mMasterVolume;
		float32 mMusicVolume;
		float32 mSoundEffectVolume;

		ALCdevice* mDevice;
		ALCcontext* mContext;
	};
}
