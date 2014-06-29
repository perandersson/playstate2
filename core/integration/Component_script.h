#pragma once
#include "../scene/Component.h"
#include "../scene/Updatable.h"
#include "../scene/Tickable.h"

namespace core
{
	//
	// All scriptable components implements this class.
	class ScriptableComponent : public Component, public Updatable, public Tickable
	{
		DECLARE_SCRIPT_OBJECT(Component);

	public:
		ScriptableComponent();
		virtual ~ScriptableComponent();

	// Component
	public:
		virtual void OnComponentAdded();
		virtual Updatable* GetUpdatable();
		virtual Tickable* GetTickable();
		virtual void OnAttachedToScene();
		virtual void OnDetachedFromScene();

	// ScriptObject
	public:
		bool OnRegisterObject();

	// Updatable
	public:
		virtual void Update();

	// Tickable
	public:
		virtual void Tick();

	private:
		std::shared_ptr<ScriptMethod> mUpdateMethodPtr;
		std::shared_ptr<ScriptMethod> mTickMethodPtr;

		std::shared_ptr<ScriptMethod> mOnAttachedToSceneMethodPtr;
		std::shared_ptr<ScriptMethod> mOnDetachedFromSceneMethodPtr;
	};
}

int Component_Factory(struct lua_State* L);
int Component_Init(struct lua_State* L);
int Component_GetSceneNode(struct lua_State* L);
int Component_GetSceneGroup(struct lua_State* L);

static luaL_Reg Component_Methods[] = {
	{ SCRIPT_CONSTRUCTOR, Component_Factory },
	{ SCRIPT_INHERIT_CONSTRUCTOR, Component_Init },
	{ "GetSceneNode", Component_GetSceneNode },
	{ "GetSceneGroup", Component_GetSceneGroup },
	{ nullptr, nullptr }
};
