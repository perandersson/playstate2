#pragma once
#include "../typedefs.h"
#include "IScriptType.h"
#include "ScriptUtils.h"
#include "ClassLoader.h"
#include "ScriptMethod.h"

namespace core
{
	/*!
		\brief Items accessable from a script must inherit from this class.
	*/
	class ScriptObject
	{
	public:
		virtual const char* GetClassName() const { return "ScriptObject"; }
		static const char* GetStaticClassName() { return "ScriptObject"; }
		virtual const IScriptType* GetScriptType() const { return &ClassLoader::GetType("ScriptObject"); }
		static const IScriptType* GetStaticScriptType() { return &ClassLoader::GetType("ScriptObject"); }

	public:
		ScriptObject();
		virtual ~ScriptObject();

		/*!
			\brief Attaches this object to a script ID.
		*/
		bool RegisterObject();

		/*!
			\brief Attaches this object to a script ID.

			\param scriptID
						The unique script reference for this object.
		*/
		bool RegisterObject(uint32 scriptID);

		/*!
			\brief Detaches this object from the script engine.
		*/
		void UnregisterObject();

		/*!
			\brief Checks if this instance has a script reference

			\return TRUE if this object is bound to the script engine; FALSE otherwise.
		*/
		inline bool IsScriptBound() const {
			return mScriptID != 0;
		}

		/*!
			\brief Retrieves this object's unique ID
			
			The ID might be reused by another object if this object is being unregistered.

			\return The object's unique ID
		*/
		inline const uint32 GetScriptID() const {
			return mScriptID;
		}

		/*!
			\brief Retrieves a script method with the supplied name

			\param name
						The name of the method
			\return A handle to the method
		*/
		std::shared_ptr<ScriptMethod> GetMethodPtr(const char* name);

	protected:
		/*!
			\brief Method called whenever this object is being attached to the script engine.
		
			Being attached means that the object is pushed onto the script stack

			\return TRUE if the registration is successful; FALSE otherwise
		*/
		virtual bool OnRegisterObject();

		/*!
			\brief Method called whenever this object is being detached from the script engine
		*/
		virtual void OnUnregisterObject();

	private:
		uint32 mScriptID;
	};
}
