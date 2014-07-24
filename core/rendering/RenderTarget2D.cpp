#include "../Memory.h"
#include "RenderTarget2D.h"
using namespace core;

RenderTarget2D::RenderTarget2D(GLuint textureID, const Size& size, TextureFormat::Enum format)
: Texture2D(textureID, size, format)
{

}

RenderTarget2D::~RenderTarget2D()
{

}

void RenderTarget2D::Lock()
{
	mMutex.lock();
}

void RenderTarget2D::Unlock()
{
	mMutex.unlock();
}
