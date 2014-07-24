#pragma once
#include "Texture2D.h"

namespace core
{
	class RenderTarget2D : public Texture2D
	{
	public:
		RenderTarget2D(GLuint textureID, const Size& size, TextureFormat::Enum format);
		virtual ~RenderTarget2D();
	};
}
