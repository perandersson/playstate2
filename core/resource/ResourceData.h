#pragma once
#include "ResourceObject.h"
#include <atomic>

namespace core
{
	//
	// Structure used by the resource manager to manage resources. Instances of this type is 
	// only removed whenever the resource manager is destroyed.
	struct ResourceData
	{
		std::atomic<ResourceObject*> resource;
		ResourceObject* defaultResource;
	};
}

