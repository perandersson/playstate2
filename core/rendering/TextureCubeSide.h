#pragma once
#include "../typedefs.h"
#include "../math/Vector3.h"
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

		/*!
			\brief Retrieves a OpenGL enum value based on the texture cube side
		*/
		static GLenum Parse(Enum e);

		/*!
			\brief
		*/
		static const Vector3& GetDirection(Enum e);
	};
}
