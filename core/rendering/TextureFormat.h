#pragma once
#include "../typedefs.h"

namespace core
{
	class TextureFormat
	{
	public:
		enum Enum {
			R = 0,
			RGB,
			RGBA,
			RGBA8,
			RGBA12,
			RGBA16,
			RGB10_A2,
			RGBA16F,
			BGR,
			BGRA,
			DEPTH24,
			DEPTH24_STENCIL8,

			SIZE
		};

		//
		// Parse the supplied string and return the enum representing it.
		//
		// @param s
		// @param defaultVal
		static Enum Parse(const std::string& s, Enum defaultValue);
	};
}