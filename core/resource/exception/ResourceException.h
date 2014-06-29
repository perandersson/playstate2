#pragma once
#include "../../Exception.h"

namespace core
{
	class ResourceException : public Exception
	{
	public:
		ResourceException(const char* function, const long line, const char* file);
		ResourceException(const char* function, const long line, const char* file, const char* message, ...);
		virtual ~ResourceException();
	};
}