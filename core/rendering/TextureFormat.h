#pragma once
#include "../typedefs.h"

namespace core
{
	class TextureFormat
	{
	public:
		enum Enum {
			R = 0,
			R16F,
			R32F,
			RGB,
			RGB8,
			RGB12,
			RGB16,
			RGB32,
			RGBA,
			RGBA8,
			RGBA12,
			RGBA16,
			RGB10_A2,
			RGBA16F,
			BGR,
			BGRA,
			DEPTH24,
			DEPTH32F,
			DEPTH24_STENCIL8,
			DEPTH32F_STENCIL8
		};

		/* Number of enums available */
		static const uint32 SIZE = DEPTH32F_STENCIL8 + 1;
		
		/*!
			\brief Parse the supplied string and return the enum representing it

			\param s
						The name of the enum
			\param defaultValue
						Value if the supplied string does not map to an enum
		*/
		static Enum Parse(const std::string& s, Enum defaultValue);
		
		/*!
			\brief Retrieves the values available in this enum

			\return A map matching name with enum value
		*/
		static std::hash_map<std::string, int32> GetValues();
	};
}