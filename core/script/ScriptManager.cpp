#include "../Memory.h"
#include "ScriptManager.h"
#include "../kernel/Kernel.h"
using namespace core;

std::shared_ptr<IScriptFile> ScriptManager::CompileFile(const std::shared_ptr<IFile> file)
{
	return Kernel::GetScriptManager()->CompileFile(file);
}

std::shared_ptr<IScriptFile> ScriptManager::CompileFile(const std::string& filePath)
{
	return Kernel::GetScriptManager()->CompileFile(filePath);
}

ScriptResult ScriptManager::EvaluateFile(const std::shared_ptr<IFile> file)
{
	return Kernel::GetScriptManager()->EvaluateFile(file);
}

ScriptResult ScriptManager::EvaluateFile(const std::string& filePath)
{
	return Kernel::GetScriptManager()->EvaluateFile(filePath);
}

void ScriptManager::SetGlobalVar(const char* name, float32 value)
{
	Kernel::GetScriptManager()->SetGlobalVar(name, value);
}

void ScriptManager::SetGlobalVar(const char* name, float64 value)
{
	Kernel::GetScriptManager()->SetGlobalVar(name, value);
}

lua_State* ScriptManager::Lock()
{
	return Kernel::GetScriptManager()->Lock();
}

void ScriptManager::Unlock(lua_State* L)
{
	Kernel::GetScriptManager()->Unlock(L);
}
