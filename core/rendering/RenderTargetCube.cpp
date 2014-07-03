#include "../Memory.h"
#include "RenderTargetCube.h"
using namespace core;

RenderTargetCube::RenderTargetCube(GLuint textureID, const Size& size, TextureFormat::Enum format)
: TextureCube(textureID, size, format)
{

}

RenderTargetCube::~RenderTargetCube()
{

}
