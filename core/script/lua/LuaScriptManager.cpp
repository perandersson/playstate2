#include "../../Memory.h"
#include "LuaScriptManager.h"
#include "../../filesystem/FileSystem.h"
#include "../exception/ScriptNotFoundException.h"
#include "LuaScriptFile.h"
#include "LuaScriptType.h"
#include "LuaScriptEnum.h"
#include "../ScriptUtils.h"
#include "../../logging/Logger.h"
#include "../ScriptObject.h"
using namespace core;

int __playstate_delete(lua_State* L)
{
	int32 numobjects = lua_gettop(L);
	for (int32 i = 0; i < numobjects; ++i) {
		ScriptObject* ptr = ScriptUtils::ToScriptObject(L);
		if (ptr != nullptr) {
			delete ptr;
		}
	}
	
	return 0;
}

int __playstate_lua_require(lua_State* L)
{
	std::string package = ScriptUtils::PackageToFile(lua_tostring(L, -1));
	lua_pop(L, 1);

	std::shared_ptr<IFile> file = FileSystem::OpenFile(package);
	if (file->Exists()) {
		auto value = file->Read().str();
		int res = luaL_loadstring(L, value.c_str());
		if (res != 0) {
			std::string err = lua_tostring(L, -1);
			Logger::Error(err.c_str());
			lua_pushnil(L);
		}
	}
	else {
		Logger::Error("\n\tCould not include file \"%s\". File not found", package.c_str());
		lua_pushnil(L);
	}
	return 1;
}

LuaScriptManager::LuaScriptManager(IFileSystem* fileSystem)
: mFileSystem(fileSystem), mLuaState(nullptr)
{
	mLuaState = luaL_newstate();
	luaL_openlibs(mLuaState);

	//
	// Register built-in functions used by the script engine
	//

	lua_register(mLuaState, "__playstate_require", __playstate_lua_require);
	lua_register(mLuaState, "Delete", __playstate_delete);
	
	//
	// Make sure that the require statement has support for the playstate file system
	//

	int res = luaL_dostring(mLuaState, "table.insert(package.searchers, __playstate_require)");
	if (res != 0) {
		std::string err = lua_tostring(mLuaState, -1);
		lua_pop(mLuaState, 1);
		THROW_EXCEPTION(ScriptException, "Could not register necessary 'require' function. Reason: '%s'", err.c_str());
	}
}

LuaScriptManager::~LuaScriptManager()
{
	if(mLuaState != nullptr) {
		int32 luaTop = lua_gettop(mLuaState);
		assert(luaTop == 0 && "Lua stack is not 0 which means that we have a memory leak somewhere");
		lua_close(mLuaState);
		mLuaState = nullptr;
	}
}

std::shared_ptr<IScriptFile> LuaScriptManager::CompileFile(const std::shared_ptr<IFile> file)
{
	LuaStateScopedLock lock(this);
	lua_State* L = lock.GetLuaState();

	const std::string& fileName = file->GetAbsolutePath();
	if (!file->Exists()) {
		Logger::Error("Could not find script file: '%s'", fileName.c_str());
		THROW_EXCEPTION(ScriptNotFoundException, "Could not find script file: '%s'", fileName.c_str());
	}
	//PushIdentity(scriptFile->GetPath());

	int32 stackCount = lua_gettop(L);
	auto value = file->Read().str();
	int res = luaL_loadstring(L, value.c_str());
	if (res != 0) {
		std::string err = lua_tostring(L, -1);
		lua_pop(L, 1);
		Logger::Error("Could not load file: %s. Reason: %s", fileName.c_str(), err.c_str());
		//PopIdentity();
		THROW_EXCEPTION(ScriptException, "Could not load file: %s. Reason: %s", fileName.c_str(), err.c_str());
	}

	res = lua_pcall(L, 0, LUA_MULTRET, NULL);
	if (res != 0) {
		std::string err = lua_tostring(L, -1);
		lua_pop(L, 1);
		Logger::Error("Could not compile file: %s. Reason: %s", fileName.c_str(), err.c_str());
		//PopIdentity();
		THROW_EXCEPTION(ScriptException, "Could not compile file: %s. Reason: %s", fileName.c_str(), err.c_str());
	}
	stackCount = lua_gettop(L) - stackCount;
	//assert(stackCount == 0);
	if (stackCount > 0)
		lua_pop(L, stackCount); // Make sure that the objects returned by the script is silently removed from the stack
	//PopIdentity();

	return std::shared_ptr<IScriptFile>(new LuaScriptFile(this));
}

std::shared_ptr<IScriptFile> LuaScriptManager::CompileFile(const std::string& filePath)
{
	return CompileFile(mFileSystem->OpenFile(filePath));
}

ScriptResult LuaScriptManager::EvaluateFile(const std::shared_ptr<IFile> file)
{
	LuaStateScopedLock lock(this);
	lua_State* L = lock.GetLuaState();

	const std::string& fileName = file->GetAbsolutePath();
	if (!file->Exists()) {
		Logger::Error("Could not find script file: '%s'", fileName.c_str());
		THROW_EXCEPTION(ScriptNotFoundException, "Could not find script file: '%s'", fileName.c_str());
	}

	int32 stackCount = lua_gettop(L);
	auto value = file->Read().str();
	int res = luaL_loadstring(L, value.c_str());
	if (res != 0) {
		std::string err = lua_tostring(L, -1);
		lua_pop(L, 1);
		Logger::Error("Could not load file: %s. Reason: %s", fileName.c_str(), err.c_str());
		//PopIdentity();
		THROW_EXCEPTION(ScriptException, "Could not load file: %s. Reason: %s", fileName.c_str(), err.c_str());
	}

	res = lua_pcall(L, 0, LUA_MULTRET, NULL);
	if (res != 0) {
		std::string err = lua_tostring(L, -1);
		lua_pop(L, 1);
		Logger::Error("Could not compile file: %s. Reason: %s", fileName.c_str(), err.c_str());
		//PopIdentity();
		THROW_EXCEPTION(ScriptException, "Could not compile file: %s. Reason: %s", fileName.c_str(), err.c_str());
	}
	stackCount = lua_gettop(L) - stackCount;

	return ScriptResult(this, stackCount);
}

ScriptResult LuaScriptManager::EvaluateFile(const std::string& filePath)
{
	return EvaluateFile(mFileSystem->OpenFile(filePath));
}

void LuaScriptManager::SetGlobalVar(const char* name, float32 value)
{
	LuaStateScopedLock lock(this);
	lua_State* L = lock.GetLuaState();

	lua_pushnumber(L, value);
	lua_setglobal(L, name);
}

void LuaScriptManager::SetGlobalVar(const char* name, float64 value)
{
	LuaStateScopedLock lock(this);
	lua_State* L = lock.GetLuaState();

	lua_pushnumber(L, value);
	lua_setglobal(L, name);
}

lua_State* LuaScriptManager::Lock()
{
	mMutex.lock();
	return mLuaState;
}

void LuaScriptManager::Unlock(lua_State* L)
{
	mMutex.unlock();
}

void LuaScriptManager::GC()
{
	LuaStateScopedLock lock(this);
	lua_State* L = lock.GetLuaState();

	lua_gc(L, LUA_GCSTEP, 180);
	lua_gc(L, LUA_GCSTOP, 0);
}

IScriptType& LuaScriptManager::GetType(const char* name)
{
	std::string nameAsString(name);
	ScriptTypes::iterator it = mScriptTypes.find(nameAsString);
	if (it != mScriptTypes.end()) {
		return *(it->second.get());
	}

	LuaScriptType* type = new LuaScriptType(this, nameAsString);
	mScriptTypes.insert(std::make_pair(nameAsString, std::shared_ptr<LuaScriptType>(type)));
	return *type;
}

IScriptEnum& LuaScriptManager::GetEnum(const char* name)
{
	std::string nameAsString(name);
	ScriptEnums::iterator it = mScriptEnums.find(nameAsString);
	if (it != mScriptEnums.end()) {
		return *(it->second.get());
	}

	LuaScriptEnum* type = new LuaScriptEnum(this, nameAsString);
	mScriptEnums.insert(std::make_pair(nameAsString, std::shared_ptr<LuaScriptEnum>(type)));
	return *type;
}
