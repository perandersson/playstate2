#include "../Memory.h"
#include "ClassLoader.h"
#include "../kernel/Kernel.h"
using namespace core;

IScriptType& ClassLoader::GetType(const char* name)
{
	return Kernel::GetClassLoader()->GetType(name);
}

IScriptEnum& ClassLoader::GetEnum(const char* name)
{
	return Kernel::GetClassLoader()->GetEnum(name);
}
