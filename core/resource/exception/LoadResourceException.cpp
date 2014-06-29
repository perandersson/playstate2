#include "../../Memory.h"
#include "LoadResourceException.h"
using namespace core;

LoadResourceException::LoadResourceException(const char* function, const long line, const char* file)
: ResourceException(function, line, file)
{
}

LoadResourceException::LoadResourceException(const char* function, const long line, const char* file, const char* message, ...)
: ResourceException(function, line, file)
{
	va_list arglist;
	va_start(arglist, message);
	BuildMessage(message, arglist);
	va_end(arglist);
}

LoadResourceException::~LoadResourceException()
{
}
