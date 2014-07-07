#include "../Memory.h"
#include "Texture2D.h"
#include "RenderContext.h"
#include "RenderState.h"
using namespace core;

Texture2D::Texture2D(GLuint textureID, const Size& size, TextureFormat::Enum format)
: Texture(textureID, GL_TEXTURE_2D, size, format)
{
}

Texture2D::~Texture2D()
{
}

void Texture2D::Resize(const Size& newSize)
{
	RenderContext::ResizeTexture2D(this, newSize);
}
