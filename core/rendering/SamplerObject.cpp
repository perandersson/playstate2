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

SamplerObject::SamplerObject(GLuint samplerID) 
: mUID(GenSamplerObjectUID()), mSamplerID(samplerID)
{
}

SamplerObject::~SamplerObject()
{
	if (mSamplerID != 0) {
		glDeleteSamplers(1, &mSamplerID);
		mSamplerID = 0;
	}
}
