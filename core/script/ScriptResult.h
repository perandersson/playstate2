#pragma once
#include "../typedefs.h"
#include "Table.h"
#include "ScriptUtils.h"

extern "C"
{
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}

namespace core
{
	class IScriptManager;

	/*!
		\brief Symbolises a result from a script evaluation
	*/
	class ScriptResult
	{
	public:
		ScriptResult(const ScriptResult& rhs);
		ScriptResult(IScriptManager* scriptManager, uint32 numResults);
		~ScriptResult();

		//
		// Converts the returned result at the supplied index into a collection
		//
		// @param idx
		//			The return index
		// @return A collection. If the result is not a collection then an empty collection is returned
		Table ToTable(int32 idx = -1) const;
		int32 ToInt32(int32 idx = -1) const;
		float32 ToFloat32(int32 idx = -1) const;
		bool ToBool(int32 idx = -1) const;
		std::string ToString(int32 idx = -1) const;
		Color ToColor(int32 idx = -1) const;
		Vector2 ToVector2(int32 idx = -1) const;
		Point ToPoint(int32 idx = -1) const;
		Size ToSize(int32 idx = -1) const;

		template<class T>
		Resource<T> ToResource(int32 idx = -1) const {
			if (mNumResults == 0)
				return Resource<T>();

			LuaStateScopedLock lock(mScriptManager);
			lua_State* L = lock.GetLuaState();

			return ScriptUtils::ToResource<T>(L, idx, false);
		}

		template<class T>
		T* ToObject(int32 idx = -1) const {
			if (mNumResults == 0)
				return nullptr;

			LuaStateScopedLock lock(mScriptManager);
			lua_State* L = lock.GetLuaState();

			return ScriptUtils::ToObject<T>(L, idx, false);
		}
		
		/*!
			\brief Retrieves the number of results returned
			
			\return The number of results returned
		*/
		uint32 GetNumResults() const {
			return mNumResults;
		}

	private:
		IScriptManager* mScriptManager;
		uint32 mNumResults;
	};
}
