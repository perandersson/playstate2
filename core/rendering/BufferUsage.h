#pragma once
#include "../typedefs.h"
#include "GLEWMX.h"

namespace core
{
	class BufferUsage
	{
	public:
		enum Enum {
			STATIC = 0,
			DYNAMIC,

			SIZE
		};

		/*!
			\brief Converts this enum into a GLenum
		*/
		static GLenum Parse(Enum e);
	};
}
