#pragma once
#include "../typedefs.h"

namespace core
{
	//
	// Base class for events sent between nodes and components in the game engine
	struct Event
	{
		Event() {}
		virtual ~Event() {}
	};
}
