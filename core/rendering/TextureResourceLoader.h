#pragma once
#include "RenderContextResourceLoader.h"

namespace core
{
	class TextureResourceLoader : public RenderContextResourceLoader
	{
	public:
		TextureResourceLoader();
		virtual ~TextureResourceLoader();

	public:
		virtual ResourceObject* Load(const IFile* file);
		virtual ResourceObject* GetDefaultResource();
		virtual bool WatchForFileSystemChanges() const { return true; }

	private:
		ResourceObject* mDefaultResource;
	};
}
