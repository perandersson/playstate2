#pragma once
#include "../typedefs.h"
#include "ScriptResult.h"

namespace core
{
	class IScriptFile
	{
	public:
		//
		// Execute the supplied expression
		//
		// @param expression
		virtual ScriptResult Execute(const char* expression) = 0;
	};
}
