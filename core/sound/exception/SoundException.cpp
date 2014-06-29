#include "../../Memory.h"
#include "SoundException.h"
using namespace core;

SoundException::SoundException(const char* function, const long line, const char* file)
: Exception(function, line, file)
{
}

SoundException::SoundException(const char* function, const long line, const char* file, const char* message, ...)
: Exception(function, line, file)
{
	va_list arglist;
	va_start(arglist, message);
	BuildMessage(message, arglist);
	va_end(arglist);
}

SoundException::~SoundException()
{
}
