#pragma once
#include "../../resource/IResourceLoader.h"

namespace core
{
	class OpenALMusicResourceLoader : public IResourceLoader
	{
	public:
		OpenALMusicResourceLoader();
		virtual ~OpenALMusicResourceLoader();

	public:
		virtual ResourceObject* Load(const IFile* file);
		virtual ResourceObject* GetDefaultResource();
		virtual bool WatchForFileSystemChanges() const { return false; }
	};

}
