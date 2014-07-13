#include "../Memory.h"
#include "SamplerObject.h"
#include <atomic>
using namespace core;

namespace {
	uint32 GenSamplerObjectUID() {
		static std::atomic<uint32> ids(0);
		return ++ids;
	}
}

SamplerObject::SamplerObject(GLuint samplerID, MinFilter::Enum minFilter, MagFilter::Enum magFilter,
	TextureWrap::Enum wraps, TextureWrap::Enum wrapt, TextureWrap::Enum wrapr) : mUID(GenSamplerObjectUID()),
	mSamplerID(samplerID), mMinFilter(minFilter), mMagFilter(magFilter), mWrapS(wraps), mWrapT(wrapt), mWrapR(wrapr)
{
}

SamplerObject::~SamplerObject()
{
	if (mSamplerID != 0) {
		glDeleteSamplers(1, &mSamplerID);
		mSamplerID = 0;
	}
}
