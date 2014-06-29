#pragma once
#include "../math/Vector2.h"
#include "FontCharInfo.h"
#include "FontAlign.h"
#include "../resource/ResourceObject.h"
#include "../rendering/Texture2D.h"

namespace core
{
	/*!
		\brief Resource representing a font
	*/
	class Font : public ResourceObject
	{
	public:
		typedef std::hash_map<int8, FontCharInfo*> FontCharInfoMap;

	public:
		Font(Texture2D* texture, const FontCharInfoMap& infoMap, float32 spaceWidth, float32 lineHeight);
		virtual ~Font();

		/*!
			\return The texture representation of this font
		*/
		inline Texture2D* GetTexture() const {
			return mTexture;
		}

		/*!
			\brief Calculates and returns the length of the supplied string

			\param str
						The string we want to calculate the string for
			\return The length
		*/
		Vector2 GetSizeOfString(const std::string& str) const;

		/*!
			\brief Calculates and returns the length of the supplied string

			Calculates and returns the length of the supplied string. If the string is longer than maxLength
			then it will put the new characters on a new line.

			\param str
						The string
			\param maxLength
						The maximum length (pixels in width)
			\return The length
		*/
		Vector2 GetSizeOfString(const std::string& str, uint32 maxLength) const;

		/*!
			\brief Retrieves information about a character

			\param c
						The character
			\return Information about the supplied character. Returns a "[]" character if the supplied character wasn't one of the loaded ones.
		*/
		const FontCharInfo& GetFontCharInfo(int32 c) const;

		/*!
			\brief Update this font so that the supplied characters are loaded into the font memory.

			\param characters
						The characters that we want to load into memory
		*/
		void Update(const std::string& characters);

		/*!
			\return The width of a space character
		*/
		inline float32 GetSpaceWidth() const {
			return mSpaceWidth;
		}

		/*!
			\return The height of a new line
		*/
		inline float32 GetLineHeight() const {
			return mLineHeight;
		}

	private:
		Texture2D* mTexture;
		FontCharInfoMap mInfo;
		FontCharInfo mCharInfoNotFound;
		float32 mSpaceWidth;
		float32 mLineHeight;
	};
}
