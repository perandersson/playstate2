#pragma once
#include "../Music.h"
#include "../../LinkedList.h"
#include <AL/al.h>

namespace core
{
	class IOpenALMusicStream
	{
	public:
		virtual ~IOpenALMusicStream() {}

		//
		// Streams music from this instance and put the result in the supplied OpenAL buffer
		//
		// @param buffer The target buffer
		// @param loop If the stream should be looping
		// @return TRUE if more data can be streamed from the supplied data; FALSE otherwise.
		virtual bool Stream(ALuint buffer, bool loop) = 0;

		//
		// Resets the stream
		virtual void Reset() = 0;
	};

	//
	// 
	class OpenALMusic : public Music
	{
	public:
		LinkedListLink<OpenALMusic> MusicLink;

	public:
		OpenALMusic(SoundFormat::Enum format, float32 duration, std::vector<ALuint> buffers, IOpenALMusicStream* stream);
		virtual ~OpenALMusic();

		//
		// @return TRUE if this music instance is being played already.
		inline bool IsPlaying() const {
			return MusicLink.IsLinked();
		}

		//
		// Attaches this music resource to a source
		//
		// @param source
		// @param looping
		void AttachToSource(ALuint source, bool looping);

		//
		// Detaches this music instance from it's attached source
		//
		void DetachFromSource();

		//
		// Update the buffers with new music data
		//
		// @return TRUE if more music are available for processing.
		// @throws playstate::SoundException If an OpenAL error is raised while updating the queued buffers
		bool UpdateStream();

	private:
		ALuint mAttachSourceID;
		bool mLooping;
		std::vector<ALuint> mBuffers;
		IOpenALMusicStream* mStream;
	};
}
