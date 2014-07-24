#pragma once
#include "../resource/ResourceObject.h"
#include "../math/Point.h"
#include "TextureFormat.h"
#include "GLEWMX.h"

namespace core
{
	/*!
		\brief Base class for all textures used by the rendering engine
	*/
	class Texture : public ResourceObject
	{
	public:
		Texture(GLuint textureID, GLenum textureTarget, const Size& size, TextureFormat::Enum format);
		virtual ~Texture();

		//
		// @return A unique ID for this texture
		inline uint32 GetUID() const {
			return mUID;
		}

		//
		// @return The internal OpenGL texture ID
		inline GLuint GetTextureID() const {
			return mTextureID;
		}

		inline GLenum GetTextureTarget() const {
			return mTextureTarget;
		}

		//
		// @return The size of the texture
		inline const Size& GetSize() const {
			return mSize;
		}

		//
		// @return The texture format
		inline TextureFormat::Enum GetTextureFormat() const {
			return mTextureFormat;
		}

	protected:
		uint32 mUID;
		GLuint mTextureID;
		GLenum mTextureTarget;
		Size mSize;
		TextureFormat::Enum mTextureFormat;
	};
}
