#include "TextureCube.h"
using namespace core;

TextureCube::TextureCube(GLuint textureID, const Size& size, TextureFormat::Enum format)
: Texture(textureID, GL_TEXTURE_CUBE_MAP, size, format)
{
}

TextureCube::~TextureCube()
{
}
