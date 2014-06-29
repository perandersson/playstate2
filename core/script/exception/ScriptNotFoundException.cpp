#include "../../Memory.h"
#include "ScriptNotFoundException.h"
using namespace core;

ScriptNotFoundException::ScriptNotFoundException(const char* function, const long line, const char* file)
: ScriptException(function, line, file)
{
}

ScriptNotFoundException::ScriptNotFoundException(const char* function, const long line, const char* file, const char* message, ...)
: ScriptException(function, line, file)
{
	va_list arglist;
	va_start(arglist, message);
	BuildMessage(message, arglist);
	va_end(arglist);
}

ScriptNotFoundException::~ScriptNotFoundException()
{
}
