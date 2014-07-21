#pragma once
#include "../typedefs.h"

namespace core
{
	class MinFilter
	{
	public:
		enum Enum {
			NEAREST = 0,
			LINEAR,
			NEAREST_MIPMAP_NEAREST,
			LINEAR_MIPMAP_NEAREST,
			NEAREST_MIPMAP_LINEAR,
			LINEAR_MIPMAP_LINEAR
		};

		/* Default min filter used by the rendering engine */
		static const Enum DEFAULT = LINEAR;

		/* Number enums available */
		static const uint32 SIZE = LINEAR_MIPMAP_LINEAR + 1;
		
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
