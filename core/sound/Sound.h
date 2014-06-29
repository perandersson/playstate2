#pragma once
#include "../resource/ResourceObject.h"

namespace core
{
	class SoundFormat
	{
	public:
		enum Enum {
			MONO8 = 0,
			MONO16,
			STEREO8,
			STEREO16,

			SIZE
		};
	};

	class Sound : public ResourceObject
	{
	public:
		Sound(SoundFormat::Enum format, float32 duration);
		virtual ~Sound();

		//
		// @return This sound effects format
		inline SoundFormat::Enum GetFormat() const {
			return mFormat;
		}

		//
		// @return This sound effects duration in seconds.
		inline float32 GetDuration() const {
			return mDuration;
		}

	private:
		float32 mDuration;
		SoundFormat::Enum mFormat;
	};
}
