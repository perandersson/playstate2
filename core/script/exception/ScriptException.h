#pragma once
#include "../../Exception.h"

namespace core
{
	class ScriptException : public Exception
	{
	public:
		ScriptException(const char* function, const long line, const char* file);
		ScriptException(const char* function, const long line, const char* file, const char* message, ...);
		virtual ~ScriptException();
	};
}