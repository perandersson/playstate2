#include "../Memory.h"
#include "TextureResourceLoader.h"
#include "Texture2D.h"
#include "IRenderContext.h"
#include "exception/RenderingException.h"
#include "../filesystem/FileSystem.h"
#include <FreeImage.h>
using namespace core;

TextureResourceLoader::TextureResourceLoader()
: RenderContextResourceLoader(), mDefaultResource(nullptr)
{
}

TextureResourceLoader::~TextureResourceLoader()
{
	if (mDefaultResource != nullptr) {
		delete mDefaultResource;
		mDefaultResource = nullptr;
	}
}

ResourceObject* TextureResourceLoader::Load(const IFile* file)
{
	const size_t fileSize = file->GetFileSize();
	if (fileSize == 0)
		return nullptr;
	std::shared_ptr<byte> pixels = file->ReadBytes();
	
	FIMEMORY* hmem = FreeImage_OpenMemory(pixels.get(), fileSize);
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeFromMemory(hmem, 0);
	FIBITMAP* dib = nullptr;

	// Check that the plugin has reading capabilities and load the file
	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_LoadFromMemory(fif, hmem, 0);

	if (dib == nullptr) {
		FreeImage_CloseMemory(hmem);
		return nullptr;
	}

	BYTE* bits = FreeImage_GetBits(dib);
	const uint32 width = FreeImage_GetWidth(dib);
	const uint32 height = FreeImage_GetHeight(dib);
	const uint32 bpp = FreeImage_GetBPP(dib);
	if ((bits == 0) || (width == 0) || (height == 0)) {
		FreeImage_CloseMemory(hmem);
		return nullptr;
	}

	TextureFormat::Enum format = TextureFormat::BGR;
	if (bpp == 32) {
		format = TextureFormat::BGRA;
	}

	auto renderContext = GetRenderContext();
	try {
		auto texture = renderContext->CreateTexture2D(Size(width, height), format, bits);
		FreeImage_Unload(dib);
		FreeImage_CloseMemory(hmem);
		return texture;
	}
	catch (RenderingException e) {
		FreeImage_Unload(dib);
		FreeImage_CloseMemory(hmem);
		THROW_EXCEPTION(LoadResourceException, "Could not load texture: '%s'. Reason: '%s'", file->GetAbsolutePath().c_str(), e.GetMessage().c_str());
	}
}

ResourceObject* TextureResourceLoader::GetDefaultResource()
{
	if (!mDefaultResource) {
		auto file = FileSystem::OpenFile("/engine/defaults/texture2d.png");
		if (!file->Exists())
			THROW_EXCEPTION(LoadResourceException, "Could not load default resource: '/engine/defaults/texture2d.png'");

		mDefaultResource = TextureResourceLoader::Load(file.get());
	}
	return mDefaultResource;
}
