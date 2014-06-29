#include "../../Memory.h"
#include "OpenALSoundEngine.h"
#include "OpenALMusic.h"
#include "OpenALSoundEffect.h"
#include "../../configuration/IConfiguration.h"
#include "../../logging/Logger.h"
using namespace core;

OpenALSoundEngine::OpenALSoundEngine(IConfiguration* configuration)
	: mMusicResources(offsetof(OpenALMusic, MusicLink)), mNextMusicSourceIndex(0), mNumSources(0), mNextSourceIndex(0), mMasterVolume(1.0f), 
	mMusicVolume(0.5f), mSoundEffectVolume(0.5f), mDevice(NULL), mContext(NULL)
{
	mDevice = alcOpenDevice(NULL);
	mContext = alcCreateContext(mDevice, NULL);
	alcMakeContextCurrent(mContext);

	// Try to generate as many sources as possible
	memset(mMusicSources, 0, sizeof(mMusicSources));
	alGenSources(MaxMusicSources, mMusicSources);

	memset(mSources, 0, sizeof(mSources));
	for(uint32 i = 0; i < MaxSoundSources; ++i) {
		alGenSources(1, &mSources[i]);
		mNumSources++;
	}
	
	static const float32 velocity[] = {0.0f, 0.0f, 0.0f};
	alListenerfv(AL_VELOCITY, velocity);
	static const float32 orientation[] = {0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f};
	alListenerfv(AL_ORIENTATION, orientation);

	SetMasterVolume(configuration->ToFloat32("sound.mastervolume", 1.0f));
	SetMusicVolume(configuration->ToFloat32("sound.musicvolume", 0.5f));
	SetSoundEffectVolume(configuration->ToFloat32("sound.soundeffectvolume", 1.0f));
}

OpenALSoundEngine::~OpenALSoundEngine()
{
	// Stop all sound sources
	alSourceStopv(MaxMusicSources, mMusicSources);
	alSourceStopv(MaxSoundSources, mSources);

	alDeleteSources(MaxMusicSources, mMusicSources);
	alDeleteSources(mNumSources, mSources);

	alcMakeContextCurrent(NULL);
    alcDestroyContext(mContext);
	mContext = NULL;
    alcCloseDevice(mDevice);
	mDevice = NULL;
}

void OpenALSoundEngine::UpdateStreamingBuffers()
{
	OpenALMusic* music = mMusicResources.First();
	while(music != NULL) {
		OpenALMusic* tmp = music->MusicLink.Tail;
		music->UpdateStream();
		music = tmp;
	}
}

void OpenALSoundEngine::Play(Resource<Music> music)
{
	OpenALSoundEngine::Play(music.Get());
}

void OpenALSoundEngine::Play(Resource<Music> music, float32 fadeInTime)
{
	OpenALSoundEngine::Play(music.Get(), fadeInTime);
}

void OpenALSoundEngine::Stop(Resource<Music> music, float32 fadeOutTime)
{
	OpenALSoundEngine::Stop(music.Get(), fadeOutTime);
}

void OpenALSoundEngine::Play(Music* music)
{
	assert_not_null(music);

	OpenALMusic* oalMusic = static_cast<OpenALMusic*>(music);
	if(oalMusic->IsPlaying())
		return;

	mMusicResources.AddLast(oalMusic);

	const ALuint source = FindNextMusicSource();
	if (source == 0) {
		Logger::Error("Could not find a free music source");
		return;
	}
	oalMusic->AttachToSource(source, true);

	// Set position at a relative position to the player so that
	// the sound has a "non-positioned" sound
	alSource3i(source, AL_POSITION, 0, 0, -1);
	alSourcei(source, AL_SOURCE_RELATIVE, AL_TRUE);
    alSourcei(source, AL_ROLLOFF_FACTOR, 0);
    alSourcei(source, AL_LOOPING, AL_FALSE);
}

void OpenALSoundEngine::Play(Music* music, float32 fadeInTime)
{
	assert_not_implemented();
}


void OpenALSoundEngine::Stop(Resource<Music> music)
{
	OpenALSoundEngine::Stop(music.Get());
}

void OpenALSoundEngine::Stop(Music* music)
{
	OpenALMusic* oalMusic = static_cast<OpenALMusic*>(music);
	if(!oalMusic->IsPlaying())
		return;

	oalMusic->DetachFromSource();
}

void OpenALSoundEngine::Stop(Music* music, float32 fadeOutTime)
{
	assert_not_implemented();
}

void OpenALSoundEngine::Play(const Resource<SoundEffect> effect)
{
	OpenALSoundEngine::Play(effect.Get());
}

void OpenALSoundEngine::Play(const Resource<SoundEffect> effect, const Vector3& position)
{
	OpenALSoundEngine::Play(effect.Get(), position);
}

void OpenALSoundEngine::Play(const SoundEffect* effect)
{
	assert_not_null(effect);
	const ALuint source = FindNextSource();
	if (source == 0) {
		Logger::Error("Could not find a free sound source");
		return;
	}

	alSource3i(source, AL_POSITION, 0, 0, 0);
	alSourcei(source, AL_SOURCE_RELATIVE, AL_TRUE);
	PlaySoundEffectBuffer(source, static_cast<const OpenALSoundEffect*>(effect)->GetBufferID());
}

void OpenALSoundEngine::Play(const SoundEffect* effect, const Vector3& position)
{
	assert_not_null(effect);
	const SoundFormat::Enum format = effect->GetFormat();
	if(format != SoundFormat::MONO8 && format != SoundFormat::MONO16) {
		Logger::Warn("The supplied sound effect is not a MONO sound. 3D positioning is disabled for this effect.");
	}

	const ALuint source = FindNextSource();
	if (source == 0) {
		Logger::Error("Could not find a free sound source");
		return;
	}

	alSource3f(source, AL_POSITION, position.x, position.y, position.z);
	alSourcei(source, AL_SOURCE_RELATIVE, AL_FALSE);
	PlaySoundEffectBuffer(source, static_cast<const OpenALSoundEffect*>(effect)->GetBufferID());
}

void OpenALSoundEngine::PlaySoundEffectBuffer(ALuint sourceID, ALuint bufferID)
{
	alSourcef(sourceID, AL_PITCH, 1.0f);
	alSourcef(sourceID, AL_GAIN, mSoundEffectVolume * mMasterVolume);
    alSourcei(sourceID, AL_LOOPING, AL_FALSE);
	alSourcei(sourceID, AL_BUFFER, bufferID);
	alSource3f(sourceID, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
	alSourcePlay(sourceID);
	auto error = alGetError();
	assert(error  == AL_NO_ERROR && "Could not set source parameters and play buffer");
}

void OpenALSoundEngine::SetMasterVolume(float32 volume)
{
	volume = volume > 1.0f ? 1.0f : volume;
	volume = volume < 0.0f ? 0.0f : volume;

	mMasterVolume = volume;

	// Update all music sources with the new volume
	for(uint32 i = 0; i < MaxMusicSources; ++i) {
		alSourcef(mMusicSources[i], AL_GAIN, mMusicVolume * mMasterVolume);
	}
}

void OpenALSoundEngine::SetMusicVolume(float32 volume)
{
	volume = volume > 1.0f ? 1.0f : volume;
	volume = volume < 0.0f ? 0.0f : volume;
	
	mMusicVolume = volume;
	
	// Update all existing music instances with the new volume
	for(uint32 i = 0; i < MaxMusicSources; ++i) {
		alSourcef(mMusicSources[i], AL_GAIN, mMusicVolume * mMasterVolume);
	}
}

void OpenALSoundEngine::SetSoundEffectVolume(float32 volume)
{
	volume = volume > 1.0f ? 1.0f : volume;
	volume = volume < 0.0f ? 0.0f : volume;

	mSoundEffectVolume = volume;
}

ISoundReceiver* OpenALSoundEngine::GetSoundReceiver(uint32 index)
{
	assert(index == 0 && "Only one sound receiver is available at the moment");
	return this;
}

void OpenALSoundEngine::SetPosition(const Vector3& position)
{
	alListener3f(AL_POSITION, position.x, position.y, position.z);
	alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
}

void OpenALSoundEngine::LookAt(const Vector3& direction, const Vector3& up)
{
	const float32 orientation[] = { direction.x, direction.y, direction.z, up.x, up.y, up.z };
	alListenerfv(AL_ORIENTATION, orientation);
}

ALuint OpenALSoundEngine::FindNextSource()
{
	for (uint32 sourceIndex = 0; sourceIndex < MaxSoundSources; ++sourceIndex) {
		const ALuint source = mSources[sourceIndex];
		ALenum state;
		alGetSourcei(source, AL_SOURCE_STATE, &state);
		if (state != AL_PLAYING) {
			return mSources[sourceIndex];
		}
	}

	return 0;
}

ALuint OpenALSoundEngine::FindNextMusicSource()
{
	for (uint32 sourceIndex = 0; sourceIndex < MaxMusicSources; ++sourceIndex) {
		const ALuint source = mMusicSources[sourceIndex];
		ALenum state;
		alGetSourcei(source, AL_SOURCE_STATE, &state);
		if (state != AL_PLAYING) {
			return mMusicSources[sourceIndex];
		}
	}

	return 0;
}
