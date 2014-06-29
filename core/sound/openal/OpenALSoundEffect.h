#pragma once
#include "../SoundEffect.h"
#include <AL/al.h>

namespace core
{
	class OpenALSoundEffect : public SoundEffect
	{
	public:
		OpenALSoundEffect(SoundFormat::Enum format, float32 duration, ALuint bufferID);
		virtual ~OpenALSoundEffect();

		//
		// @return
		inline ALuint GetBufferID() const {
			return mBufferID;
		}

	private:
		ALuint mBufferID;
	};
}
