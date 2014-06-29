#include "../Memory.h"
#include "Logger.h"
#include "../kernel/Kernel.h"
using namespace core;

void Logger::Error(const char* msg, ...)
{
	va_list arglist;
	va_start(arglist, msg);
	char tmp[5096];
	vsprintf_s(tmp, 5096, msg, arglist);
	va_end(arglist);

	Kernel::GetLogger()->Error(tmp);
}

void Logger::Info(const char* msg, ...)
{
	va_list arglist;
	va_start(arglist, msg);
	char tmp[5096];
	vsprintf_s(tmp, 5096, msg, arglist);
	va_end(arglist);

	Kernel::GetLogger()->Info(tmp);
}

void Logger::Debug(const char* msg, ...)
{
	va_list arglist;
	va_start(arglist, msg);
	char tmp[5096];
	vsprintf_s(tmp, 5096, msg, arglist);
	va_end(arglist);

	Kernel::GetLogger()->Debug(tmp);
}

void Logger::Warn(const char* msg, ...)
{
	va_list arglist;
	va_start(arglist, msg);
	char tmp[5096];
	vsprintf_s(tmp, 5096, msg, arglist);
	va_end(arglist);

	Kernel::GetLogger()->Warn(tmp);
}
