#pragma once
#include "../typedefs.h"
#include "GLEWMX.h"

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
			NONE,

			SIZE
		};

		static GLenum Parse(Enum e);
	};
}
