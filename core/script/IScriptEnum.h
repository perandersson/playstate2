#pragma once
#include "../typedefs.h"

namespace core
{
	class ScriptObject;

	//
	//
	class IScriptEnum
	{
	public:
		virtual ~IScriptEnum() {}

	public:
		virtual const std::string& GetName() const = 0;
		virtual IScriptEnum& Enum(const char* name, int32 value) = 0;
		virtual IScriptEnum& Enums(const std::hash_map<std::string, int32>& enums) = 0;
		virtual void Build() = 0;
	};
}
