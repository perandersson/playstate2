#pragma once
#include "ScriptException.h"

namespace core
{
	class ScriptNotFoundException : public ScriptException
	{
	public:
		ScriptNotFoundException(const char* function, const long line, const char* file);
		ScriptNotFoundException(const char* function, const long line, const char* file, const char* message, ...);
		virtual ~ScriptNotFoundException();
	};
}
