#pragma once
#include "../typedefs.h"

namespace core
{
	class TextureWrap
	{
	public:
		enum Enum {
			CLAMP_TO_EDGE = 0,
			CLAMP_TO_BORDER,
			MIRRORED_REPEAT,
			REPEAT
		};

		/* Default texture wrap used by the rendering engine */
		static const Enum DEFAULT = REPEAT;

		/* Number enums available */
		static const uint32 SIZE = REPEAT + 1;
		
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
