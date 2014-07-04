#pragma once
#include "../typedefs.h"

namespace core
{
	/*!
		\brief
	*/
	class TextureCubeSide
	{
	public:
		enum Enum {
			POSITIVE_X = 0,
			NEGATIVE_X,
			POSITIVE_Y,
			NEGATIVE_Y,
			POSITIVE_Z,
			NEGATIVE_Z,

			ALL,

			SIZE
		};
	};
}
