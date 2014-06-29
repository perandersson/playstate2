#pragma once
#include "../typedefs.h"
#include "../math/Color.h"
#include "../math/Vector2.h"
#include "../math/Vector3.h"
#include "../math/Point.h"
#include "../math/Rect.h"
#include "../resource/Resource.h"
#include "Table.h"

extern "C"
{
   #include <lua.h>
   #include <lauxlib.h>
   #include <lualib.h>
}

namespace core
{
	class ScriptObject;

	class ScriptUtils
	{
	public:
		//
		// Converts a package into a file on the file system
		//
		// @param package
		// @return
		static std::string PackageToFile(const std::string& package);

		//
		// Read a float from the supplied lua state.
		//
		// @param L
		//			The lua state
		// @param idx
		//			The position where the float value is located
		// @param removeFromStack
		//			TRUE if the value should be removed from the stack; FALSE otherwise
		static float32 ToFloat32(lua_State* L, int idx = -1, bool removeFromStack = true);

		//
		// Pushes a float to the supplied lua state's stack
		static int PushFloat32(lua_State* L, float32 value);

		//
		// Read a 32-bit integer from the supplied lua state.
		//
		// @param L
		//			The lua state
		// @param idx
		//			The position where the 32-bit integer value is located
		// @param removeFromStack
		//			TRUE if the value should be removed from the stack; FALSE otherwise
		static int32 ToInt32(lua_State* L, int idx = -1, bool removeFromStack = true);

		//
		// Pushes a 32-bit integer to the supplied lua state's stack
		static int PushInt32(lua_State* L, int32 value);

		//
		// Read a 32-bit unsigned integer from the supplied lua state.
		//
		// @param L
		//			The lua state
		// @param idx
		//			The position where the 32-bit integer value is located
		// @param removeFromStack
		//			TRUE if the value should be removed from the stack; FALSE otherwise
		static uint32 ToUInt32(lua_State* L, int idx = -1, bool removeFromStack = true);

		//
		// Pushes a 32-bit unsigned integer to the supplied lua state's stack
		static int PushUInt32(lua_State* L, uint32 value);

		//
		// Read a typeask from the supplied lua state.
		//
		// @param L
		//			The lua state
		// @param idx
		//			The position where the 32-bit integer value is located
		// @param removeFromStack
		//			TRUE if the value should be removed from the stack; FALSE otherwise
		static typemask ToTypeMask(lua_State* L, int idx = -1, bool removeFromStack = true);

		//
		// Pushes a typemask to the supplied lua state's stack
		static int PushTypeMask(lua_State* L, typemask value);

		//
		// Retrieves a string from the stack at the supplied index
		//
		// @param L
		// @parma idx
		// @param removeFromStack
		static std::string ToString(lua_State* L, int idx = -1, bool removeFromStack = true);

		//
		// Pushes a string value to the supplied lua state's stack
		static int PushString(lua_State* L, const std::string& value);

		//
		// Retrieves a user data object from the stack at the supplied index
		//
		// @param L
		// @parma idx
		// @param removeFromStack
		static void* ToPointer(lua_State* L, int idx = -1, bool removeFromStack = true);

		//
		// Pushes pointer to the supplied lua state's stack
		static int PushPointer(lua_State* L, const void* ptr);
		
		//
		// Retrieves a resource data object from the stack at the supplied index
		//
		// @param L
		// @parma idx
		// @param removeFromStack
		static ResourceData* ToResourceData(lua_State* L, int idx = -1, bool removeFromStack = true);

		//
		// Pushes a resource data object to the supplied lua state's stack
		static int PushResourceData(lua_State* L, ResourceData* value);

		//
		// Pushes a resource to the supplied lua state's stack
		template<typename T>
		static void PushResource(lua_State* L, Resource<T>& resource) {
			PushResourceData(L, resource.GetResourceData());
		}

		//
		// Retrieves a resource from the stack at the supplied index
		//
		// @param L
		// @parma idx
		// @param removeFromStack
		template<typename T>
		static Resource<T> ToResource(lua_State* L, int idx = -1, bool removeFromStack = true) {
			ResourceData* resourceData = ToResourceData(L, idx, removeFromStack);
			if (resourceData != nullptr) {
				return Resource<T>(resourceData);
			}
			return Resource<T>();
		}

		//
		// Retrieves a boolean value from the stack at the supplied index
		//
		// @param L
		// @parma idx
		// @param removeFromStack
		static bool ToBool(lua_State* L, int idx = -1, bool removeFromStack = true);

		//
		// Pushes a boolean to the supplied lua state's stack
		static int PushBool(lua_State* L, bool value);

		//
		// Retrieves a 2d-vector value from the stack at the supplied index
		//
		// @param L
		// @parma idx
		// @param removeFromStack
		static Vector2 ToVector2(lua_State* L, int idx = -1, bool removeFromStack = true);

		//
		// Pushes a 2d vector to the supplied lua state's stack
		static uint32 PushVector2(lua_State* L, const Vector2& vec);

		//
		// Retrieves a 3d-vector value from the stack at the supplied index
		//
		// @param L
		// @parma idx
		// @param removeFromStack
		static Vector3 ToVector3(lua_State* L, int idx = -1, bool removeFromStack = true);

		//
		// Pushes a 3d vector to the supplied lua state's stack
		static int PushVector3(lua_State* L, const Vector3& vec);

		//
		// Retrieves a color from the stack at the supplied index
		//
		// @param L
		// @parma idx
		// @param removeFromStack
		static Color ToColor(lua_State* L, int idx = -1, bool removeFromStack = true);

		//
		// Pushes a color to the supplied lua state's stack
		static int PushColor(lua_State* L, const Color& color);

		//
		// Retrieves a point from the stack at the supplied index
		//
		// @param L
		// @parma idx
		// @param removeFromStack
		static Point ToPoint(lua_State* L, int idx = -1, bool removeFromStack = true);

		//
		// Pushes a 3d vector to the supplied lua state's stack
		static int PushPoint(lua_State* L, const Point& vec);

		//
		// Retrieves a rect from the stack at the supplied index. 
		//
		// @param L
		// @parma idx
		// @param removeFromStack
		static Rect ToRect(lua_State* L, int idx = -1, bool removeFromStack = true);

		//
		// Retrieves a script object from the stack at the supplied index. 
		//
		// @param L
		// @parma idx
		// @param removeFromStack
		static ScriptObject* ToScriptObject(lua_State* L, int idx = -1, bool removeFromStack = true);

		//
		// Pushes a script object to the supplied lua state's stack
		static int PushObject(lua_State* L, ScriptObject* object);

		//
		// Log an error
		static void LogError(lua_State* L, const char* message);

		//
		//
		static void SetObject(lua_State* L, ScriptObject* object);

		//
		// Retrieves a script object from the of the stack
		//
		// @param L
		// @param idx
		// @param removeFromStack
		template<class T>
		static T* ToObject(lua_State* L, int idx = -1, bool removeFromStack = true) {
			ScriptObject* object = ToScriptObject(L, idx, removeFromStack);
			if (object != nullptr) {
				return dynamic_cast<T*>(object);
			}
			return nullptr;
		}

		//
		// Retrieves an enum from the of the stack
		//
		// @param L
		// @param idx
		// @param removeFromStack
		template<class Enum>
		static Enum ToEnum(lua_State* L, int idx = -1, bool removeFromStack = true) {
			uint32 value = ToUInt32(L, idx, removeFromStack);
			return (Enum)value;
		}
	};
}
