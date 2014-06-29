#pragma once
#include "../typedefs.h"
#include "../StringUtils.h"
#include "ClassLoader.h"

extern "C"
{
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}

namespace core
{
	class ScriptObject;

	/*!
		\brief The IScriptType interface 
	*/
	class IScriptType
	{
	public:
		virtual ~IScriptType() {}

	public:
		virtual const std::string& GetName() const = 0;
		virtual IScriptType& AddFunction(const char* name, lua_CFunction function) = 0;
		virtual IScriptType& AddFunctions(luaL_Reg* methods) = 0;
		virtual IScriptType& InheritFrom(const char* name) = 0;
		virtual IScriptType& InheritFrom(const IScriptType& type) = 0;
		virtual uint32 RegisterObject(ScriptObject* object) const = 0;
		virtual uint32 RegisterObject(ScriptObject* object, uint32 scriptID) const = 0;
		virtual void UnregisterObject(ScriptObject* object) const = 0;
		virtual void Build() = 0;
	};

	/*!
		ID where the pointer is located inside the weak reference
	*/
	static const char ScriptObjectRefID[] = { "__oref" };
	
	/*!
		ID where the object type is located inside the weak reference
	*/
	static const char ScriptObjectTypeID[] = { "__otype" };
}

#ifndef DECLARE_SCRIPT_OBJECT
#define DECLARE_SCRIPT_OBJECT(ClazzName) public: \
	virtual const char* GetClassName() const { return #ClazzName; } \
	static const char* GetStaticClassName() { return #ClazzName; } \
	virtual const IScriptType* GetScriptType() const { return &core::ClassLoader::GetType(#ClazzName); } \
	static const IScriptType* GetStaticScriptType() { return &core::ClassLoader::GetType(#ClazzName); }
#endif

#ifndef DEFINE_SCRIPT_OBJECT
#define DEFINE_SCRIPT_OBJECT(Clazz)
#endif

#define SCRIPT_CONSTRUCTOR "__call"
#define SCRIPT_INHERIT_CONSTRUCTOR "__init"
#define SCRIPT_TOSTRING "__tostring"
#define SCRIPT_GC "__gc"
