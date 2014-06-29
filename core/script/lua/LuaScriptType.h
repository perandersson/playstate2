#pragma once
#include "../IScriptType.h"

namespace core
{
	class LuaScriptManager;

	class LuaScriptType : public IScriptType
	{
		typedef std::hash_map<std::string, lua_CFunction> Functions;

	public:
		LuaScriptType(LuaScriptManager* scriptManager, const std::string& name);
		virtual ~LuaScriptType();

	public:
		virtual const std::string& GetName() const;
		virtual IScriptType& AddFunction(const char* name, lua_CFunction function);
		virtual IScriptType& AddFunctions(luaL_Reg* methods);
		virtual IScriptType& InheritFrom(const char* name);
		virtual IScriptType& InheritFrom(const IScriptType& type);
		virtual uint32 RegisterObject(ScriptObject* object) const;
		virtual uint32 RegisterObject(ScriptObject* object, uint32 scriptID) const;
		virtual void UnregisterObject(ScriptObject* object) const;
		virtual void Build();

	private:
		LuaScriptManager* mScriptManager;
		std::string mName;
		Functions mFunctions;
	};
}
