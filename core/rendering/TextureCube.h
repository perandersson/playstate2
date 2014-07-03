#pragma once
#include "Texture.h"

namespace core
{
	/*!
		\brief Class symbolizing a cube texture (i.e. a texture having 6 sides with the same sizes)
	*/
	class TextureCube : public Texture
	{
	public:
		TextureCube(GLuint textureID, const Size& size, TextureFormat::Enum format);
		virtual ~TextureCube();
	};
}
