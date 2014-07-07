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
			DYNAMIC
		};

		/*!
			\brief The size of this enum
		*/
		static const uint32 SIZE = Enum::DYNAMIC + 1;

		/*!
			\brief Converts this enum into a GLenum
		*/
		static GLenum Parse(Enum e);
	};
}
