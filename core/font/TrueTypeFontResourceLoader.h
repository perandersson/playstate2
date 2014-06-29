#pragma once
#include "../rendering/RenderContextResourceLoader.h"
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>

namespace core
{
	class TrueTypeFontResourceLoader : public RenderContextResourceLoader
	{
	public:
		TrueTypeFontResourceLoader();
		virtual ~TrueTypeFontResourceLoader();

		// IResourceLoader
	public:
		virtual ResourceObject* Load(const IFile* file);
		virtual ResourceObject* GetDefaultResource();
		virtual bool WatchForFileSystemChanges() const { return false; }

	private:
		int GetNextP2(int a) const;
		void CopyToBuffer(uint32 s, uint32 t, uint32 width, uint32 height, byte* target, const byte* src);

	private:
		FT_Library mLibrary;
	};
}
