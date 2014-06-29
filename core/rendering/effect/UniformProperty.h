#pragma once
#include "../../typedefs.h"
#include "../MagFilter.h"
#include "../MinFilter.h"
#include "../TextureWrap.h"
#include <gl/glew.h>

namespace core
{
	/*!
		\brief Information about a uniform's property
	*/
	struct UniformProperty
	{
		GLuint componentID;
		GLenum uniformType;
		std::string name;

		//
		// Texture properties
		//

		MinFilter::Enum minFilter;
		MagFilter::Enum magFilter;
		TextureWrap::Enum wraps;
		TextureWrap::Enum wrapt;

		UniformProperty() {
			minFilter = MinFilter::DEFAULT;
			magFilter = MagFilter::DEFAULT;
			wraps = TextureWrap::DEFAULT;
			wrapt = TextureWrap::DEFAULT;
		}
	};

}
