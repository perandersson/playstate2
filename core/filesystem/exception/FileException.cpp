#include "../../Memory.h"
#include "FileException.h"
using namespace core;

FileException::FileException(const char* function, const long line, const char* file)
: Exception(function, line, file)
{
}

FileException::FileException(const char* function, const long line, const char* file, const char* message, ...)
: Exception(function, line, file)
{
	va_list arglist;
	va_start(arglist, message);
	BuildMessage(message, arglist);
	va_end(arglist);
}

FileException::~FileException()
{
}
