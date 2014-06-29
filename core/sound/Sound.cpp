#include "../Memory.h"
#include "Sound.h"
using namespace core;

Sound::Sound(SoundFormat::Enum format, float32 duration)
: ResourceObject(), mFormat(format), mDuration(duration)
{
}

Sound::~Sound()
{
}
