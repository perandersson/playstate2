#pragma once
#include "IScriptFile.h"
#include "LuaStateScopedLock.h"
#include "exception/ScriptException.h"

extern "C"
{
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}

namespace core
{
	class IFile;

	class IScriptManager
	{
	public:
		virtual ~IScriptManager() {}

	public:
		//
		// Compile the supplied file and return the result.
		//
		// @param file
		//			The script file we want to compile
		virtual std::shared_ptr<IScriptFile> CompileFile(const std::shared_ptr<IFile> file) = 0;

		//
		// Compile the supplied file and return the result
		//
		// @param filePath
		//			The path to the file we want to compile
		virtual std::shared_ptr<IScriptFile> CompileFile(const std::string& filePath) = 0;

		//
		// Compile the supplied file and return the result.
		//
		// @param file
		//			The script file we want to compile and execute
		virtual ScriptResult EvaluateFile(const std::shared_ptr<IFile> file) = 0;

		//
		// Evaluates the supplied file and return the result
		//
		// @param filePath
		//			The path to the file we want to compile end execute
		virtual ScriptResult EvaluateFile(const std::string& filePath) = 0;

		//
		// Set a global value for the current lua state
		//
		// @param name
		// @param value
		virtual void SetGlobalVar(const char* name, float32 value) = 0;

		//
		// Set a global value for the current lua state
		//
		// @param name
		// @param value
		virtual void SetGlobalVar(const char* name, float64 value) = 0;

		//
		// Lock the lua state for the current thread and return a pointer to it
		//
		// @return A pointer to the current lua state.
		virtual lua_State* Lock() = 0;

		//
		// Unlocks the lua state and return it back to the script manager
		//
		// @param L
		//			The lua state
		virtual void Unlock(lua_State* L) = 0;
	};

}
