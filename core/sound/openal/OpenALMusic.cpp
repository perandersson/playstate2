#include "../../Memory.h"
#include "OpenALMusic.h"
#include "../../logging/Logger.h"
#include "../exception/SoundException.h"
using namespace core;

OpenALMusic::OpenALMusic(SoundFormat::Enum format, float32 duration, std::vector<ALuint> buffers, IOpenALMusicStream* stream)
: Music(format, duration), mBuffers(buffers), mStream(stream)
{
	assert_not_null(stream);
}

OpenALMusic::~OpenALMusic()
{
	if (mBuffers.size() > 0) {
		alDeleteBuffers(mBuffers.size(), &mBuffers[0]);
		mBuffers.clear();
	}

	if (mStream != nullptr) {
		delete mStream;
		mStream = nullptr;
	}
}

void OpenALMusic::AttachToSource(ALuint source, bool looping)
{
	assert(looping == true && "Non-looping music is not working at the moment");

	mAttachSourceID = source;
	mLooping = looping;

	const uint32 numBuffers = mBuffers.size();
	for (uint32 i = 0; i < numBuffers; ++i) {
		if (!mStream->Stream(mBuffers[i], false)) {
			THROW_EXCEPTION(SoundException, "Could not fill buffer stream");
		}
	}

	alSourceQueueBuffers(source, numBuffers, &mBuffers[0]);
	alSourcePlay(source);
}

void OpenALMusic::DetachFromSource()
{
	alSourceStop(mAttachSourceID);
	mAttachSourceID = 0;
	mLooping = false;
	MusicLink.Unlink();
}

bool OpenALMusic::UpdateStream()
{
	int processed = 0;
	bool active = true;

	ALenum sourceInfo;
	alGetSourcei(mAttachSourceID, AL_SOURCE_STATE, &sourceInfo);
	if (sourceInfo != AL_PLAYING) {
		Logger::Warn("Audio buffer under-run! Re-playing source.");
		alSourcePlay(mAttachSourceID);
	}

	int32 buffersProcessed;
	alGetSourcei(mAttachSourceID, AL_BUFFERS_PROCESSED, &buffersProcessed);

	for (int32 i = 0; i < buffersProcessed && active; ++i) {
		ALuint buffer;
		alSourceUnqueueBuffers(mAttachSourceID, 1, &buffer);
		if (alGetError() != AL_NO_ERROR) {
			THROW_EXCEPTION(SoundException, "Could not unqueue music buffer from source");
		}

		active = mStream->Stream(buffer, mLooping);
		alSourceQueueBuffers(mAttachSourceID, 1, &buffer);

		if (alGetError() != AL_NO_ERROR) {
			THROW_EXCEPTION(SoundException, "Could not queue music buffer to source");
		}
	}

	return active;
}
