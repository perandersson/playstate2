#pragma once
#include "../RenderContextResourceLoader.h"
#include "Effect.h"

namespace core
{
	class EffectResourceLoader : public RenderContextResourceLoader
	{
	public:
		EffectResourceLoader();
		virtual ~EffectResourceLoader();

	public:
		virtual ResourceObject* Load(const IFile* file);
		virtual ResourceObject* GetDefaultResource();
		virtual bool WatchForFileSystemChanges() const { return true; }
	};
}
