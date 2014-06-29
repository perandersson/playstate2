#pragma once
#include "Sound.h"

namespace core
{
	class SoundEffect : public Sound
	{
	public:
		SoundEffect(SoundFormat::Enum format, float32 duration);
		virtual ~SoundEffect();
	};
}