#include "../Memory.h"
#include "Texture.h"
#include <atomic>
using namespace core;

namespace {
	uint32 GenTextureUID() {
		static std::atomic<uint32> ids(0);
		return ++ids;
	}
}

Texture::Texture(GLuint textureID, GLenum textureTarget, const Size& size, TextureFormat::Enum format)
: ResourceObject(), mUID(GenTextureUID()), mTextureID(textureID), mTextureTarget(textureTarget), mSize(size), mTextureFormat(format)
{
}

Texture::~Texture()
{
	if (mTextureID != 0) {
		glDeleteTextures(1, &mTextureID);
		mTextureID = 0;
	}
}
