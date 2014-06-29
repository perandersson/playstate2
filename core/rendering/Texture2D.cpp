#include "../Memory.h"
#include "Texture2D.h"
using namespace core;

Texture2D::Texture2D(GLuint textureID, const Size& size, TextureFormat::Enum format)
: Texture(textureID, GL_TEXTURE_2D, size, format)
{
}

Texture2D::~Texture2D()
{
}
