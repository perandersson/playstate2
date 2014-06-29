#pragma once
#include "Sound.h"

namespace core
{
	//
	//
	class Music : public Sound
	{
	public:
		Music(SoundFormat::Enum format, float32 duration);
		virtual ~Music();
	};
}