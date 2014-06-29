#pragma once

namespace core
{
	class IScriptType;
	class IScriptEnum;

	class IClassLoader
	{
	public:
		virtual ~IClassLoader() {}
		
		//
		// Get or create a script type with the supplied name
		//
		// @param name
		// @return
		virtual IScriptType& GetType(const char* name) = 0;

		//
		// Get or create a script enum type with the supplied name
		//
		// @param name
		// @return
		virtual IScriptEnum& GetEnum(const char* name) = 0;
	};

}
