#pragma once
#include "Texture.h"

namespace core
{
	class Texture2D : public Texture
	{
	public:
		Texture2D(GLuint textureID, const Size& size, TextureFormat::Enum format);
		virtual ~Texture2D();
	};
}
