#pragma once
#include "../typedefs.h"

namespace core
{
	// The maximum amount of render targets at the same time.
	// TODO: Make this dynamic
	static const uint32 MAX_RENDER_TARGETS = 6U;

	// The maximum amount of bound textures at the same time
	// TODO: Make this dynamic
	static const uint32 MAX_ACTIVE_TEXTURES = 16U;

}
