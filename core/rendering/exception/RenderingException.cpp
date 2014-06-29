#include "../../Memory.h"
#include "RenderingException.h"
using namespace core;

RenderingException::RenderingException(const char* function, const long line, const char* file)
	: Exception(function, line, file)
{
}

RenderingException::RenderingException(const char* function, const long line, const char* file, const char* message, ...)
	: Exception(function, line, file)
{
	va_list arglist;
    va_start(arglist, message);
	BuildMessage(message, arglist);
    va_end(arglist);
}

RenderingException::~RenderingException()
{
}
