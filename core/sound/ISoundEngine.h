#pragma once
#include "SoundEffect.h"
#include "Music.h"
#include "ISoundReceiver.h"
#include "../math/vector3.h"
#include "../resource/resource.h"

namespace core
{
	class ISoundEngine
	{
	public:
		virtual ~ISoundEngine() {}

		//
		// Plays the supplied sound effect
		//
		// @param effect
		virtual void Play(const Resource<SoundEffect> effect) = 0;

		//
		// Plays the supplied sound effect at the supplied position
		//
		// @param effect
		//			The effect we want to play
		// @param position
		//			The position where we want to play the effect at
		// @remark Playing positioned stereo sound effects are not possible. 
		//		The sound engine will print a warning message in the log and play this at the
		//		listeners position instead.
		virtual void Play(const Resource<SoundEffect> effect, const Vector3& position) = 0;

		//
		// Plays the supplied sound effect
		//
		// @param effect
		virtual void Play(const SoundEffect* effect) = 0;

		//
		// Plays the supplied sound effect at the supplied position
		//
		// @param effect
		//			The effect we want to play
		// @param position
		//			The position where we want to play the effect at
		// @remark Playing positioned stereo sound effects are not possible. 
		//		The sound engine will print a warning message in the log and play this at the
		//		listeners position instead.
		virtual void Play(const SoundEffect* effect, const Vector3& position) = 0;

		//
		// Starts playing a music resource
		//
		// @param music
		// @remark The game engine can play up to three music resources at the same time. Although not recommended for extended use,
		//	this feature greatly enhance smooth transitions between music tracks.
		virtual void Play(Resource<Music> music) = 0;

		//
		// Starts playing the supplied music resource
		//
		// @param fadeInTime
		virtual void Play(Resource<Music> music, float32 fadeInTime) = 0;

		//
		// Stop the supplied music
		virtual void Stop(Resource<Music> music) = 0;

		//
		// Stop the supplied music
		virtual void Stop(Resource<Music> music, float32 fadeOutTime) = 0;

		//
		// Starts playing a music resource
		//
		// @param music
		// @remark The game engine can play up to three music resources at the same time. Although not recommended for extended use,
		//	this feature greatly enhance smooth transitions between music tracks.
		virtual void Play(Music* music) = 0;

		//
		// Starts playing the supplied music resource
		//
		// @param fadeInTime
		virtual void Play(Music* music, float32 fadeInTime) = 0;

		//
		// Stop the supplied music
		virtual void Stop(Music* music) = 0;

		//
		// Stop the supplied music
		virtual void Stop(Music* music, float32 fadeOutTime) = 0;

		//
		// @param index
		// @return
		virtual ISoundReceiver* GetSoundReceiver(uint32 index) = 0;
	};
}
