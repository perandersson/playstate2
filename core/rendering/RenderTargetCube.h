#pragma once
#include "TextureCube.h"

namespace core
{
	class RenderTargetCube : public TextureCube
	{
	public:
		RenderTargetCube(GLuint textureID, const Size& size, TextureFormat::Enum format);
		virtual ~RenderTargetCube();
	};
}
