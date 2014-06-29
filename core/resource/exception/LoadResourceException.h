#pragma once
#include "ResourceException.h"

namespace core
{
	class LoadResourceException : public ResourceException
	{
	public:
		LoadResourceException(const char* function, const long line, const char* file);
		LoadResourceException(const char* function, const long line, const char* file, const char* message, ...);
		virtual ~LoadResourceException();
	};
}