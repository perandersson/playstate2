#include "../../Memory.h"
#include "ScriptException.h"
using namespace core;

ScriptException::ScriptException(const char* function, const long line, const char* file)
: Exception(function, line, file)
{
}

ScriptException::ScriptException(const char* function, const long line, const char* file, const char* message, ...)
: Exception(function, line, file)
{
	va_list arglist;
	va_start(arglist, message);
	BuildMessage(message, arglist);
	va_end(arglist);
}

ScriptException::~ScriptException()
{
}
