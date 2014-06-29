#pragma once
#include "../typedefs.h"
#include "MinFilter.h"
#include "MagFilter.h"
#include "TextureWrap.h"
#include <gl/glew.h>

namespace core
{
	class SamplerObject
	{
	public:
		SamplerObject(GLuint samplerID, MinFilter::Enum minFilter, MagFilter::Enum magFilter,
			TextureWrap::Enum wraps, TextureWrap::Enum wrapt);
		~SamplerObject();

		inline uint32 GetUID() const {
			return mUID;
		}

		inline GLuint GetSamplerID() const {
			return mSamplerID;
		}

		void Bind(uint32 textureIndex);

	private:
		uint32 mUID;
		GLuint mSamplerID;

		MinFilter::Enum mMinFilter;
		MagFilter::Enum mMagFilter;
		TextureWrap::Enum mWrapS;
		TextureWrap::Enum mWrapT;
	};
}
