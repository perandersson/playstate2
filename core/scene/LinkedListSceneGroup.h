#pragma once
#include "SceneGroup.h"

namespace core
{
	class LinkedListSceneGroup : public SceneGroup
	{
		DECLARE_SCRIPT_OBJECT(LinkedListSceneGroup);

	public:
		LinkedListSceneGroup();
		virtual ~LinkedListSceneGroup();

		virtual void AddUpdatable(Updatable* updatable);
		virtual void RemoveUpdatable(Updatable* updatable);
		virtual void AddTickable(Tickable* tickable);
		virtual void RemoveTickable(Tickable* tickable);
		virtual void AddRenderable(Renderable* renderable);
		virtual void RemoveRenderable(Renderable* renderable);
		virtual void AddLightSource(LightSource* lightSource);
		virtual void RemoveLightSource(LightSource* lightSource);

		virtual void UpdateUpdatables();
		virtual void UpdateTickables();

		virtual bool Find(const FindQuery& query, RenderBlockResultSet* _out_resultSet) const;
		virtual bool Find(const FindQuery& query, LightSourceResultSet* _out_resultSet) const;

	private:
		LinkedList<Renderable> mRenderables;
		LinkedList<LightSource> mLightSources;
		LinkedList<Updatable> mUpdatables;
		LinkedList<Tickable> mTickables;
	};
}
