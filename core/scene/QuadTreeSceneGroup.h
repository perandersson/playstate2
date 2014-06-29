#pragma once
#include "SceneGroup.h"
#include "tree/quadtree.h"

namespace core
{
	/*!
		\brief A scene group that put all it's nodes into a quadtree structure.

		This type of scene group is recommended if we use a lot of nodes
	*/
	class QuadTreeSceneGroup : public SceneGroup
	{
		DECLARE_SCRIPT_OBJECT(QuadTreeSceneGroup);

	public:
		QuadTreeSceneGroup();
		virtual ~QuadTreeSceneGroup();

		/*!
			\brief Set the quad tree properties

			\param boundingBox
			\param maxDepth
		*/
		void SetQuadTree(const Vector3& position, float32 width, float32 height, float32 depth, uint32 maxDepth);

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
		QuadTree* mQuadTree;

		LinkedList<Renderable> mRenderables;
		LinkedList<LightSource> mLightSources;
		LinkedList<Updatable> mUpdatables;
		LinkedList<Tickable> mTickables;
	};
}
