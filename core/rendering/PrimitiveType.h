#pragma once
#include "../typedefs.h"

namespace core
{
	class PrimitiveType
	{
	public:
		enum Enum {
			POINT = 0,
			TRIANGLE,
			TRIANGLE_STRIP,
			LINE_LOOP
		};

		/* Number of enums available */
		static const uint32 SIZE = LINE_LOOP + 1;

		/*!
			\brief Retrieves the number of elements the enum represents

			\param type
			\return
		*/
		static uint32 GetElementCount(Enum type);
	};
}
