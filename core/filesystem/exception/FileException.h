#pragma once
#include "../../Exception.h"

namespace core
{
	class FileException : public Exception
	{
	public:
		FileException(const char* function, const long line, const char* file);
		FileException(const char* function, const long line, const char* file, const char* message, ...);
		virtual ~FileException();
	};
}
