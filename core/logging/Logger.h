#pragma once
#include "ILogger.h"
#include <stdarg.h>

namespace core
{
	class Logger
	{
	public:
		static void Error(const char* msg, ...);
		static void Info(const char* msg, ...);
		static void Debug(const char* msg, ...);
		static void Warn(const char* msg, ...);
	};
}
