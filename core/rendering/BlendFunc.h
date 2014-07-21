#pragma once
#include "../typedefs.h"

namespace core
{
	class SrcFactor
	{
	public:
		enum Enum {
			ZERO = 0,
			ONE,
			SRC_COLOR,
			ONE_MINUS_SRC_COLOR,
			SRC_ALPHA,
			ONE_MINUS_SRC_ALPHA,
			DST_ALPHA,
			ONE_MINUS_DST_ALPHA,
			DST_COLOR,
			ONE_MINUS_DST_COLOR,
			CONSTANT_COLOR,
			ONE_MINUS_CONSTANT_COLOR,
			CONSTANT_ALPHA,
			ONE_MINUS_CONSTANT_ALPHA
		};

		/* Default source factor used by the rendering engine */
		static const Enum DEFAULT = ONE;

		/* The number of enums available */
		static const uint32 SIZE = ONE_MINUS_CONSTANT_ALPHA + 1;
		
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

	class DestFactor
	{
	public:
		enum Enum {
			ZERO = 0,
			ONE,
			SRC_COLOR,
			ONE_MINUS_SRC_COLOR,
			SRC_ALPHA,
			ONE_MINUS_SRC_ALPHA,
			DST_ALPHA,
			ONE_MINUS_DST_ALPHA,
			DST_COLOR,
			ONE_MINUS_DST_COLOR,
			CONSTANT_COLOR,
			ONE_MINUS_CONSTANT_COLOR,
			CONSTANT_ALPHA,
			ONE_MINUS_CONSTANT_ALPHA
		};

		/* Default destination factor used by the rendering engine */
		static const Enum DEFAULT = ZERO;
		
		/* The number of enums available */
		static const uint32 SIZE = ONE_MINUS_CONSTANT_ALPHA + 1;

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

	struct BlendFunc
	{
		SrcFactor::Enum sfactor;
		DestFactor::Enum dfactor;
	};
}
