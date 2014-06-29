#include "../../Memory.h"
#include "ResourceException.h"
using namespace core;

ResourceException::ResourceException(const char* function, const long line, const char* file)
: Exception(function, line, file)
{
}

ResourceException::ResourceException(const char* function, const long line, const char* file, const char* message, ...)
: Exception(function, line, file)
{
	va_list arglist;
	va_start(arglist, message);
	BuildMessage(message, arglist);
	va_end(arglist);
}

ResourceException::~ResourceException()
{
}
