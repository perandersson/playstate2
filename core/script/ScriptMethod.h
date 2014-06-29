#pragma once
#include "ScriptUtils.h"
#include "ScriptResult.h"

namespace core
{
	/*!
		\brief The ScriptMethod class symbolizes a method registered inside the scripting engine
	*/
	class ScriptMethod
	{
	public:
		ScriptMethod();
		ScriptMethod(IScriptManager* scriptManager, uint32 scriptID, uint32 methodID);
		~ScriptMethod();

		//
		// Invoke this method and return a result
		ScriptResult Invoke();

		/*!
			\brief Checks if this method exists or not
			
			\return TRUE if this method exists; FALSE otherwise
		*/
		inline bool Exists() const {
			return mMethodID != 0;
		}

	private:
		//
		// Prepare this method
		//
		// @return TRUE if the method could be successfully bound; FALSE otherwise
		bool PrepareMethod(struct lua_State* L);

	private:
		IScriptManager* mScriptManager;
		uint32 mScriptID;
		uint32 mMethodID;
	};
}
