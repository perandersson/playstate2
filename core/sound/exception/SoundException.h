#pragma once

#include "../../Exception.h"

namespace core
{
	class SoundException : public Exception
	{
	public:
		SoundException(const char* function, const long line, const char* file);
		SoundException(const char* function, const long line, const char* file, const char* message, ...);
		virtual ~SoundException();
	};
}
