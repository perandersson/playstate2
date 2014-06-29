#include "../Memory.h"
#include "TrueTypeFontResourceLoader.h"
#include "Font.h"
#include "../rendering/exception/RenderingException.h"
using namespace core;

TrueTypeFontResourceLoader::TrueTypeFontResourceLoader()
: RenderContextResourceLoader(), mLibrary(NULL)
{
	if (FT_Init_FreeType(&mLibrary)) {
		THROW_EXCEPTION(LoadResourceException, "Could not initialize freetype library");
	}
}

TrueTypeFontResourceLoader::~TrueTypeFontResourceLoader()
{
	FT_Done_FreeType(mLibrary);
	mLibrary = nullptr;
}

ResourceObject* TrueTypeFontResourceLoader::Load(const IFile* file)
{
	static const std::string values("()*+-/0123456789=.?@ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz");

	// Get the size of the font file
	const size_t fileSize = file->GetFileSize();

	// Read the bytes from the file
	std::shared_ptr<byte> bytes = file->ReadBytes();

	// File name, used whenever we are throwing an exception
	const std::string& fileName = file->GetAbsolutePath();

	FT_Face face;
	FT_Error error = FT_New_Memory_Face(mLibrary, bytes.get(), fileSize, 0, &face);
	if (error == FT_Err_Unknown_File_Format) {
		THROW_EXCEPTION(LoadResourceException, "Unknown face format for freetype file: '%s'", fileName.c_str());
	}
	else if (error) {
		THROW_EXCEPTION(LoadResourceException, "Could not initialize freetype face. for file: '%s'", fileName.c_str());
	}

	const uint32 px = 12;
	error = FT_Set_Char_Size(face, px << 6, px << 6, 96, 96);
	if (error) {
		THROW_EXCEPTION(LoadResourceException, "Invalid freetype char size for file: '%s'", fileName.c_str());
	}

	const uint32 textureWidth = 1024;
	const uint32 textureHeight = 1024;
	std::shared_ptr<byte> copiedBytes(new byte[textureWidth * textureHeight * 2], TDeleteArray<byte>());

	uint32 offsetX = 0;
	uint32 offsetY = 0;
	uint32 highestHeightOnRow = 0;
	Font::FontCharInfoMap infoMap;
	for (uint32 i = 0; i < values.length(); ++i) {
		const std::string::value_type ch = values[i];
		if (FT_Load_Glyph(face, FT_Get_Char_Index(face, ch), FT_LOAD_DEFAULT)) {
			// ERROR
		}

		FT_Glyph glyph;
		if (FT_Get_Glyph(face->glyph, &glyph)) {
			// ERROR
		}

		FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
		FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

		const FT_Bitmap& bitmap = bitmap_glyph->bitmap;
		const uint32 bitmapHeight = (uint32)bitmap.rows;
		const uint32 bitmapWidth = (uint32)bitmap.width;

		// Texture offset 
		if (offsetX + bitmapWidth > textureWidth) {
			offsetX = 0;
			offsetY += highestHeightOnRow;
		}

		CopyToBuffer(offsetX, offsetY, bitmapWidth, bitmapHeight, copiedBytes.get(), bitmap.buffer);

		// Create character description
		FontCharInfo* info = new FontCharInfo();
		info->size = Vector2(bitmapWidth, bitmapHeight);
		info->offset = Vector2(0, px - (face->glyph->metrics.horiBearingY >> 6));
		info->bottomRightTexCoord = Vector2((offsetX + bitmapWidth) / (float32)textureWidth, offsetY / (float32)textureHeight);
		info->topLeftTexCoord = Vector2(offsetX / (float32)textureWidth, (offsetY + bitmapHeight) / (float32)textureHeight);
		infoMap.insert(std::make_pair(ch, info));

		offsetX += bitmap.width;
		highestHeightOnRow = highestHeightOnRow < bitmapHeight ? bitmapHeight : highestHeightOnRow;

		FT_Done_Glyph(glyph);
	}

	const float32 lineHeight11 = face->height >> 6;
	const float32 lineHeight = px / 0.63f;
	FT_Done_Face(face);

	try {
		Texture2D* texture = GetRenderContext()->CreateTexture2D(Size(textureWidth, textureHeight), TextureFormat::R, copiedBytes.get());
		return new Font(texture, infoMap, px / 2.0f, lineHeight);
	}
	catch (RenderingException e) {
		THROW_EXCEPTION(LoadResourceException, "Could not load texture: '%s'. Reason: '%s'", fileName.c_str(), e.GetMessage().c_str());
	}
}

void TrueTypeFontResourceLoader::CopyToBuffer(uint32 x, uint32 y, uint32 width, uint32 height, byte* target, const byte* src)
{
	for (int32 t = height - 1; t >= 0; --t) {
		for (uint32 s = 0; s < width; ++s) {
			const uint32 targetOffset = (s + x) + (t + y) * 1024;
			target[targetOffset] = *src++;
		}
	}
}

int TrueTypeFontResourceLoader::GetNextP2(int a) const
{
	int val = 2;
	while (val < a) val <<= 1;
	return val;
}

ResourceObject* TrueTypeFontResourceLoader::GetDefaultResource()
{
	return nullptr;
}
