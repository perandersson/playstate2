#include "../Memory.h"
#include "Music.h"
using namespace core;

Music::Music(SoundFormat::Enum format, float32 duration)
: Sound(format, duration)
{
}

Music::~Music()
{
}
