#pragma once
#include "../math/Vector2.h"

namespace core
{
	struct FontCharInfo
	{
		// The size of the actual character (width and height)
		Vector2 size;

		// This characters offset in X,Y. Needed so that the text looks nice.
		Vector2 offset;

		// The texture coordinates for this character
		Vector2 bottomRightTexCoord;
		Vector2 topLeftTexCoord;
	};
}
