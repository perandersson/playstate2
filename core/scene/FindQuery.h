#pragma once
#include "../camera/camera.h"

namespace core
{
	//
	// Structure which defines a search query used by the game engine. 
	struct FindQuery
	{
		//
		// Search for items from the supplied cameras perspective
		const Camera* camera;

		// 
		// Specifies what type of filter 
		typemask filter;
	};
}
