#pragma once
#include "../typedefs.h"

namespace core
{
	class PolygonMode
	{
	public:
		enum Enum {
			POINT = 0,
			LINE,
			FILL,

			SIZE
		};

		static const Enum DEFAULT = FILL;
		
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
