#include "../Memory.h"
#include "Font.h"
using namespace core;

Font::Font(Texture2D* texture, const FontCharInfoMap& infoMap, float32 spaceWidth, float32 lineHeight)
: mTexture(texture), mInfo(infoMap), mSpaceWidth(spaceWidth), mLineHeight(lineHeight)
{
	assert(texture != NULL && "You are not allowed to create a NULL texture font");
}

Font::~Font()
{
	FontCharInfoMap::iterator it = mInfo.begin();
	FontCharInfoMap::iterator end = mInfo.end();
	for (; it != end; ++it) {
		delete it->second;
	}

	delete mTexture;
	mTexture = NULL;
}

Vector2 Font::GetSizeOfString(const std::string& str) const
{
	float32 currentWidth = 0.f;
	Vector2 totalSize;
	std::string::size_type numChars = str.length();
	for (std::string::size_type i = 0; i < numChars; ++i) {
		std::string::value_type c = str[i];
		if (c == '\n') {
			totalSize.y += mLineHeight;
			currentWidth = 0;
			continue;
		}
		else if (c == '\r') {
			continue; // ignore
		}
		else if (c == ' ') {
			totalSize.x += mSpaceWidth;
			continue;
		}
		const FontCharInfo& info = GetFontCharInfo(c);

		currentWidth += info.size.y + info.offset.x;
		if (totalSize.x < currentWidth)
			totalSize.x = currentWidth;
	}

	return totalSize;
}

Vector2 Font::GetSizeOfString(const std::string& str, uint32 maxLength) const
{
	assert(false && "NOT IMPLEMENTED");
	return Vector2::ZERO;
}

const FontCharInfo& Font::GetFontCharInfo(int32 c) const
{
	FontCharInfoMap::const_iterator it = mInfo.find(c);
	if (it == mInfo.end()) {
		return mCharInfoNotFound;
	}
	return *it->second;
}


void Font::Update(const std::string& characters)
{
	assert(false && "NOT IMPLEMENTED");
}
