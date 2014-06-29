#include "../Memory.h"
#include "SoundEffect.h"
using namespace core;

SoundEffect::SoundEffect(SoundFormat::Enum format, float32 duration)
: Sound(format, duration)
{

}

SoundEffect::~SoundEffect()
{

}
