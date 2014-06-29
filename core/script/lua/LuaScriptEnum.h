#pragma once
#include "../IScriptEnum.h"

namespace core
{
	class LuaScriptManager;

	class LuaScriptEnum : public IScriptEnum
	{
		typedef std::hash_map<std::string, int32> EnumMap;

	public:
		LuaScriptEnum(LuaScriptManager* scriptManager, const std::string& name);
		virtual ~LuaScriptEnum();

	public:
		virtual const std::string& GetName() const;
		virtual IScriptEnum& Enum(const char* name, int32 value);
		virtual IScriptEnum& Enums(const std::hash_map<std::string, int32>& enums);
		virtual void Build();

	private:
		LuaScriptManager* mScriptManager;
		std::string mName;
		EnumMap mEnums;
	};
}
