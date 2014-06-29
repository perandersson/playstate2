#include "../Memory.h"
#include "SoundEngine.h"
#include "../kernel/Kernel.h"
using namespace core;

void SoundEngine::Play(const Resource<SoundEffect> effect)
{
	Kernel::GetSoundEngine()->Play(effect);
}

void SoundEngine::Play(const Resource<SoundEffect> effect, const Vector3& position)
{
	Kernel::GetSoundEngine()->Play(effect, position);
}

void SoundEngine::Play(const SoundEffect* effect)
{
	Kernel::GetSoundEngine()->Play(effect);
}

void SoundEngine::Play(const SoundEffect* effect, const Vector3& position)
{
	Kernel::GetSoundEngine()->Play(effect, position);
}

void SoundEngine::Play(Resource<Music> music)
{
	Kernel::GetSoundEngine()->Play(music);
}

void SoundEngine::Play(Resource<Music> music, float32 fadeInTime)
{
	Kernel::GetSoundEngine()->Play(music, fadeInTime);
}

void SoundEngine::Stop(Resource<Music> music)
{
	Kernel::GetSoundEngine()->Stop(music);
}

void SoundEngine::Stop(Resource<Music> music, float32 fadeOutTime)
{
	Kernel::GetSoundEngine()->Stop(music, fadeOutTime);
}

void SoundEngine::Play(Music* music)
{
	Kernel::GetSoundEngine()->Play(music);
}

void SoundEngine::Play(Music* music, float32 fadeInTime)
{
	Kernel::GetSoundEngine()->Play(music, fadeInTime);
}

void SoundEngine::Stop(Music* music)
{
	Kernel::GetSoundEngine()->Stop(music);
}

void SoundEngine::Stop(Music* music, float32 fadeOutTime)
{
	Kernel::GetSoundEngine()->Stop(music, fadeOutTime);
}

ISoundReceiver* SoundEngine::GetSoundReceiver(uint32 index)
{
	return Kernel::GetSoundEngine()->GetSoundReceiver(index);
}
