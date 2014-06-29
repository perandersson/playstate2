#pragma once

#include "../typedefs.h"
#include "../math/Color.h"
#include "../math/Vector2.h"
#include "../math/Point.h"
#include "../resource/Resource.h"

extern "C"
{
   #include <lua.h>
   #include <lauxlib.h>
   #include <lualib.h>
}

namespace core
{
	class IScriptManager;
	class ScriptObject;

	/*!
		\brief Symbolizes a script table-type.

		Useful for reading tables that originates from Lua. The table is accessed using the '.' delimiter.
		Example:
		{@code
			// LUA
			local tableValue = { 
				root = { 
					part1 = 10 
				}
			}

			// C++ code
			Table table = GetVariableFromLua("tableValue");
			int32 part1Value = table.ToInt32("root.part1");
			Log::Debug("Value: %d", part1Value); // Prints 10
		}

		It's also possible to load a value hierarchialy. The priority for which value to select is used by specifying a larger-then delimiter (>),
		where the most important value is first. Example:
		{@code
			// LUA
			local tableValue = { 
				root = { 
					part1 = 10
					subtree = {
						part1 = 20
					}
				}
			}

			// C++ code
			Table table = GetVariableFromLua("tableValue");
			int32 part1Value = table.ToInt32("root.subtree.nonexistingvalue>root.subtree.part1>root.part1");
			Log::Debug("Value: %d", part1Value); // Prints 20
		}
	*/
	class Table
	{
	public:
		Table();
		Table(IScriptManager* scriptManager, uint32 scriptRef);
		Table(const Table& rhs);
		virtual ~Table();

		Table& operator=(const Table& rhs);

	public:
		int32 ToInt32(const char* key, int32 defaultVal = 0) const;
		int32 ToInt32(const std::string& key, int32 defaultValue = 0) const;

		float32 ToFloat32(const char* key, float32 defaultVal = 0.0f) const;
		float32 ToFloat32(const std::string& key, float32 defaultVal = 0.0f) const;

		bool ToBool(const char* key, bool defaultVal = false) const;
		bool ToBool(const std::string& key, bool defaultVal = false) const;

		std::string ToString(const char* key, const char* defaultVal = "") const;
		std::string ToString(const std::string& key, const char* defaultVal = "") const;

		//
		// Find a color inside this collection and returns the result. If the
		// color wasn't found then return the supplied default color instead
		//
		// @param key
		// @param defaultColor
		Color ToColor(const char* key, const Color& defaultColor) const;
		Color ToColor(const std::string& key, const Color& defaultColor) const;

		//
		// Find a vector inside this collection.
		Vector2 ToVector2(const char* key, const Vector2& defaultVector) const;
		Vector2 ToVector2(const std::string& key, const Vector2& defaultVector) const;

		//
		// Find a vector inside this collection.
		Point ToPoint(const char* key, const Point& defaultVal) const;
		Point ToPoint(const std::string& key, const Point& defaultVal) const;

		//
		// Find a vector inside this collection.
		Size ToSize(const char* key, const Size& defaultVal) const;
		Size ToSize(const std::string& key, const Size& defaultVal) const;

		//
		// Find a resource inside this collection, for example a Font resource.
		template<class T>
		Resource<T> ToResource(const char* key) const;

		//
		// Find a resource inside this collection, for example a Font resource.
		template<class T>
		Resource<T> ToResource(const std::string& key) const;

		//
		// Find a resource inside this collection, for example a Font resource.
		template<class T>
		Resource<T> ToResource(const char* key, Resource<T> defaultValue) const;

		//
		// Find a resource inside this collection, for example a Font resource.
		template<class T>
		Resource<T> ToResource(const std::string& key, Resource<T> defaultValue) const;

		//
		// Find a scriptable instance in this collectiom.
		template<class T>
		T* ToObject(const char* key) const;

	protected:
		//
		// Find a key and pushes it to the top of the stack
		//
		// @return TRUE if the key is found; FALSE otherwise
		// @remark If no key is found then no element is pushed onto the stack
		bool FindKey(const std::string& key, lua_State* L) const;

		//
		// 
		ResourceData* FindResourceData(const std::string& key, ResourceData* defaultObject) const;

		//
		// Find a script
		ScriptObject* FindScriptObject(const std::string& key, ScriptObject* defaultObject) const;

	protected:
		IScriptManager* mScriptManager;
		uint32 mScriptRef;
	};

	template<class T>
	Resource<T> Table::ToResource(const char* key) const {
		return FindResource<T>(std::string(key));
	}

	template<class T>
	Resource<T> Table::ToResource(const std::string& key) const {
		ResourceData* data = FindResourceData(key, nullptr);
		if (data != nullptr)
			return Resource<T>(data);
		else
			return Resource<T>();
	}

	template<class T>
	Resource<T> Table::ToResource(const char* key, Resource<T> defaultValue) const
	{
		return FindResource<T>(playstate::string(key), defaultValue);
	}

	template<class T>
	Resource<T> Table::ToResource(const std::string& key, Resource<T> defaultValue) const
	{
		ResourceData* data = FindResourceData(key, defaultValue.GetResourceData());
		if (data != nullptr)
			return Resource<T>(data);
		else
			return Resource<T>();
	}

	template<class T>
	T* Table::ToObject(const char* key) const {
		ScriptObject* ptr = FindScriptObject(std::string(key), nullptr);
		if (ptr != nullptr)
			return dynamic_cast<T*>(ptr);
		else
			return nullptr;
	}
}
