#pragma once
#include "../typedefs.h"

namespace core
{
	/*!
		\brief Available mouse buttons
	*/
	class MouseButton
	{
	public:
		enum Enum {
			LEFT = 0,
			MIDDLE,
			RIGHT,

			SIZE
		};

		/*!
			\brief Retrieves the values available in this enum

			\return A map matching name with enum value
		*/
		static std::hash_map<std::string, int32> GetValues();
	};
}
