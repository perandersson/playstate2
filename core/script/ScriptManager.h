#pragma once
#include "IScriptManager.h"

namespace core
{
	class ScriptManager
	{
	public:
		static std::shared_ptr<IScriptFile> CompileFile(const std::shared_ptr<IFile> file);
		static std::shared_ptr<IScriptFile> CompileFile(const std::string& filePath);
		static ScriptResult EvaluateFile(const std::shared_ptr<IFile> file);
		static ScriptResult EvaluateFile(const std::string& filePath);
		static void SetGlobalVar(const char* name, float32 value);
		static void SetGlobalVar(const char* name, float64 value);
		static lua_State* Lock();
		static void Unlock(lua_State* L);
	};
}
