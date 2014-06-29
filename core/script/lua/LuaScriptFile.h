#pragma once
#include "../IScriptFile.h"

namespace core
{
	class LuaScriptFile : public IScriptFile
	{
	public:
		LuaScriptFile(IScriptManager* scriptManager);
		virtual ~LuaScriptFile(); 

	// IScriptFile
	public:
		virtual ScriptResult Execute(const char* expression);

	private:
		IScriptManager* mScriptManager;
	};
}