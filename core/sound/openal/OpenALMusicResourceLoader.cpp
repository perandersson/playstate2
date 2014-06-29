#include "../../Memory.h"
#include "OpenALMusicResourceLoader.h"
#include "OpenALMusic.h"
#include "../../logging/Logger.h"

#include <vorbis/codec.h>
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>
#include <ogg/ogg.h>

using namespace core;

OpenALMusicResourceLoader::OpenALMusicResourceLoader()
{
}

OpenALMusicResourceLoader::~OpenALMusicResourceLoader()
{
}

class OggVorbisMusicStream : public IOpenALMusicStream
{
	static const uint32 BufferSizeInBytes = 1024 * 32; // 32kb

public:
	OggVorbisMusicStream(std::shared_ptr<IFile> file) 
		: mOffset(0), mFileSize(file->GetFileSize()), mFormat(AL_FORMAT_MONO8), mFrequency(0), mDuration(0.0f)
	{
		mFile = file;
		memset(&mOggStream, 0, sizeof(OggVorbis_File));
		mCallbacks.read_func = OggVorbisMusicStream::AR_readOgg;
		mCallbacks.seek_func = OggVorbisMusicStream::AR_seekOgg;
		mCallbacks.close_func = OggVorbisMusicStream::AR_closeOgg;
		mCallbacks.tell_func = OggVorbisMusicStream::AR_tellOgg;
	}

	virtual ~OggVorbisMusicStream () {
		ov_clear(&mOggStream);
	}

	bool Open() {
		int ret = ov_open_callbacks((void*)this, &mOggStream, NULL, 0, mCallbacks);
		if(ret != 0)
			return false;

		vorbis_info* info  = ov_info(&mOggStream, -1);
		mDuration = (float32)ov_time_total(&mOggStream, -1);

		mFormat = AL_FORMAT_STEREO16;
		if(info->channels == 1)
			mFormat = AL_FORMAT_MONO16;

		mFrequency = info->rate;
		return true;
	}

	inline float32 GetDuration() const {
		return mDuration;
	}

	inline ALuint GetFormat() const {
		return mFormat;
	}


// IOpenALMusicStream
public:
	virtual bool Stream(ALuint buffer, bool loop) {
		int32 size = 0;
		int32 section = 0;
		int32 result = 0;

		while(size < BufferSizeInBytes) {
			result = ov_read(&mOggStream, mTemp + size, BufferSizeInBytes - size, 0, 2, 1, &section);
			if(result < 0) {
				Logger::Error("Could not continue stream music file: '%s'", mFile->GetAbsolutePath().c_str());
				return false;
			}
			
			if(result == 0) {
				if(loop) {
					Reset();
					// Prevent an infinite loop
					loop = false;
					continue;
				} else {
					break;
				}
			}

			size += result;
		}
		
		if(size == 0)
			return false;

		alBufferData(buffer, mFormat, mTemp, size, mFrequency);
		return true;
	}

	virtual void Reset() {
		ov_time_seek(&mOggStream, 0);
	}

	static size_t AR_readOgg(void* dest, size_t byteSize, size_t sizeToRead, void* fh) {
		OggVorbisMusicStream* stream = reinterpret_cast<OggVorbisMusicStream*>(fh);
		size_t sizeUntilEOF = stream->mFileSize - stream->mOffset;
		size_t bytesToRead = sizeToRead * byteSize;
		if((sizeToRead * byteSize) > sizeUntilEOF) {
			bytesToRead = sizeUntilEOF;
		}

		if(bytesToRead > 0) {
			bytesToRead = stream->mFile->ReadBytes((byte*)dest, stream->mOffset, bytesToRead);
			stream->mOffset += bytesToRead;
		}
		return bytesToRead;
	}

	static int AR_seekOgg(void* fh, ogg_int64_t to, int type) {
		OggVorbisMusicStream* stream = reinterpret_cast<OggVorbisMusicStream*>(fh);
		switch(type) {
		case SEEK_CUR:
			stream->mOffset += (uint32)to;
			break;
		case SEEK_END:
			stream->mOffset = stream->mFileSize;
			break;
		case SEEK_SET:
			stream->mOffset = (uint32)to;
			break;
		default:
			return -1;
		}

		if(stream->mOffset < 0) {
			stream->mOffset = 0;
			return -1;
		}

		if(stream->mOffset > stream->mFileSize) {
			stream->mOffset = stream->mFileSize;
			return -1;
		}

		return 0;
	}

	static int AR_closeOgg(void* fh) {
		return 0;
	}

	static long AR_tellOgg(void* fh) {
		OggVorbisMusicStream* stream = reinterpret_cast<OggVorbisMusicStream*>(fh);
		return (long)(stream->mOffset);
	}

private:
	std::shared_ptr<IFile> mFile;
	char mTemp[BufferSizeInBytes];
	uint32 mOffset;
	uint32 mFileSize;
	float32 mDuration;

	OggVorbis_File mOggStream;
	ov_callbacks mCallbacks;
	ALuint mFormat;
	ALsizei mFrequency;

};

ResourceObject* OpenALMusicResourceLoader::Load(const IFile* file)
{
	const std::string& absolutePath = file->GetAbsolutePath();
	OggVorbisMusicStream* stream = new OggVorbisMusicStream(file->OpenFile(absolutePath));
	if(!stream->Open()) {
		delete stream;
		THROW_EXCEPTION(LoadResourceException, "Could not load music file: '%s'", absolutePath.c_str());
	}

	const ALuint _format = stream->GetFormat();
	SoundFormat::Enum format = _format == AL_FORMAT_STEREO16 ? SoundFormat::STEREO16 : SoundFormat::MONO16;

	std::vector<ALuint> buffers(3);
	alGenBuffers(3, &buffers[0]);
	return new OpenALMusic(format, stream->GetDuration(), buffers, stream);
}

ResourceObject* OpenALMusicResourceLoader::GetDefaultResource()
{
	return NULL;
}
