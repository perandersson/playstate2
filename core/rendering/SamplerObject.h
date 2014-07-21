#pragma once
#include "../typedefs.h"
#include "MinFilter.h"
#include "MagFilter.h"
#include "TextureWrap.h"
#include "GLEWMX.h"

namespace core
{
	/*!
		\brief Class representing a texture sampler state

		A sampler state is responsible for calculating the actual pixel color for a texture at a give [S,T,R] coordinates
	*/
	class SamplerObject
	{
	public:
		SamplerObject(GLuint samplerID);
		~SamplerObject();

		inline uint32 GetUID() const {
			return mUID;
		}

		inline GLuint GetSamplerID() const {
			return mSamplerID;
		}

	private:
		uint32 mUID;
		GLuint mSamplerID;
	};
}
