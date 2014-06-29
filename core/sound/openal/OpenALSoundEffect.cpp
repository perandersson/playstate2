#include "../../Memory.h"
#include "OpenALSoundEffect.h"
using namespace core;

OpenALSoundEffect::OpenALSoundEffect(SoundFormat::Enum format, float32 duration, ALuint bufferID)
: SoundEffect(format, duration), mBufferID(bufferID)
{
	assert(bufferID != 0 && "Invalid buffer ID. This should not happen");
}

OpenALSoundEffect::~OpenALSoundEffect()
{
	if (mBufferID != 0) {
		alDeleteBuffers(1, &mBufferID);
		mBufferID = 0;
	}
}
