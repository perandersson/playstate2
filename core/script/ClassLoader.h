#pragma once
#include "IClassLoader.h"

namespace core
{
	class ClassLoader
	{
	public:
		static IScriptType& GetType(const char* name);
		static IScriptEnum& GetEnum(const char* name);
	};
}

