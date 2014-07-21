#pragma once
#include "../camera/Frustum.h"
#include "FindQueryFilter.h"

namespace core
{
	/*!
		\brief Structure which defines a search query used to locate objects on the scene

		You can create a default query using a structure like this:

		{@code 
			FindQuery query = { camera, FindQueryFilter::DEFAULT };
		}
	*/
	struct FindQuery
	{
		/* Search for items from the supplied cameras perspective */
		const Frustum* frustum;

		/* Specifies what we want to receive from the find query */
		typemask filter;
	};
}
