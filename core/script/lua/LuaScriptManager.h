#include "../IScriptManager.h"
#include "../IClassLoader.h"
#include <mutex>
#include <condition_variable>
#include <list>

namespace core
{
	class LuaScriptType;
	class LuaScriptEnum;
	class IFileSystem;

	class LuaScriptManager : public IScriptManager, public IClassLoader
	{
		typedef std::hash_map<std::string, std::shared_ptr<LuaScriptType>> ScriptTypes;
		typedef std::hash_map<std::string, std::shared_ptr<LuaScriptEnum>> ScriptEnums;

	public:
		LuaScriptManager(IFileSystem* fileSystem);
		virtual ~LuaScriptManager();

	// IScriptManager
	public:
		virtual std::shared_ptr<IScriptFile> CompileFile(const std::shared_ptr<IFile> file);
		virtual std::shared_ptr<IScriptFile> CompileFile(const std::string& filePath);
		virtual ScriptResult EvaluateFile(const std::shared_ptr<IFile> file);
		virtual ScriptResult EvaluateFile(const std::string& filePath);
		virtual void SetGlobalVar(const char* name, float32 value);
		virtual void SetGlobalVar(const char* name, float64 value);
		virtual lua_State* Lock();
		virtual void Unlock(lua_State* L);
		virtual void GC();

	// IScriptRegistry
	public:
		virtual IScriptType& GetType(const char* name);
		virtual IScriptEnum& GetEnum(const char* name);

	private:
		IFileSystem* mFileSystem;
		lua_State* mLuaState;
		std::recursive_mutex mMutex;
		ScriptTypes mScriptTypes;
		ScriptEnums mScriptEnums;
	};
}
