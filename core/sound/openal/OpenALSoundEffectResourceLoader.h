#pragma once
#include "../../resource/IResourceLoader.h"

namespace core
{
	class OpenALSoundEffectResourceLoader : public IResourceLoader
	{
	public:
		OpenALSoundEffectResourceLoader();
		~OpenALSoundEffectResourceLoader();

	// IResourceLoader
	public:
		virtual ResourceObject* Load(const IFile* file);
		virtual ResourceObject* GetDefaultResource();

	private:
		ResourceObject* mDefaultResource;
	};
}

