#include "../Memory.h"
#include "QuadTreeSceneGroup.h"
#include "Updatable.h"
#include "Tickable.h"
#include "light/LightSource.h"
#include "rendering/Renderable.h"
using namespace core;

QuadTreeSceneGroup::QuadTreeSceneGroup()
: SceneGroup(), 
mQuadTree(nullptr), 
mRenderables(offsetof(Renderable, RenderableLink)), mLightSources(offsetof(LightSource, LightSourceLink)),
mUpdatables(offsetof(Updatable, UpdatableLink)), mTickables(offsetof(Tickable, TickableLink))
{

}

QuadTreeSceneGroup::~QuadTreeSceneGroup()
{
	mRenderables.DeleteAll();
	mTickables.DeleteAll();
	mUpdatables.DeleteAll();
	mLightSources.DeleteAll();

	if (mQuadTree != nullptr) {
		delete mQuadTree;
		mQuadTree = nullptr;
	}
}

void QuadTreeSceneGroup::SetQuadTree(const Vector3& position, float32 width, float32 height, float32 depth, uint32 maxDepth)
{
	assert(mQuadTree == nullptr); // Transfer from the old quadtree into the new one
	mQuadTree = new QuadTree(AABB(position, width, height, depth), maxDepth);
}

void QuadTreeSceneGroup::AddUpdatable(Updatable* updatable)
{
	assert_not_null(updatable);
	mUpdatables.AddLast(updatable);
}

void QuadTreeSceneGroup::RemoveUpdatable(Updatable* updatable)
{
	assert_not_null(updatable);
	mUpdatables.Remove(updatable);
}

void QuadTreeSceneGroup::AddTickable(Tickable* tickable)
{
	assert_not_null(tickable);
	mTickables.AddLast(tickable);
}

void QuadTreeSceneGroup::RemoveTickable(Tickable* tickable)
{
	assert_not_null(tickable);
	mTickables.Remove(tickable);
}

void QuadTreeSceneGroup::AddRenderable(Renderable* renderable)
{
	assert_not_null(renderable);
	mQuadTree->Add(renderable);
	mRenderables.AddLast(renderable);
}

void QuadTreeSceneGroup::RemoveRenderable(Renderable* renderable)
{
	assert_not_null(renderable);
#ifdef _DEBUG
	assert(renderable->GetGroup() == this && "You are trying to remove someone elses renderable component");
#endif
	QuadTree* tree = static_cast<QuadTree*>(renderable->GetSpatialTree());
	tree->Remove(renderable);
	mRenderables.Remove(renderable);
}

void QuadTreeSceneGroup::AddLightSource(LightSource* lightSource)
{
	assert_not_null(lightSource);
	mLightSources.AddLast(lightSource);
}

void QuadTreeSceneGroup::RemoveLightSource(LightSource* lightSource)
{
	assert_not_null(lightSource);
	mLightSources.Remove(lightSource);
}

void QuadTreeSceneGroup::UpdateUpdatables()
{
	auto updatable = mUpdatables.First();
	while (updatable != nullptr) {
		auto next = updatable->UpdatableLink.Tail;
		updatable->Update();
		updatable = next;
	}
}

void QuadTreeSceneGroup::UpdateTickables()
{
	auto tickable = mTickables.First();
	while (tickable != nullptr) {
		auto next = tickable->TickableLink.Tail;
		tickable->Tick();
		tickable = next;
	}
}

class QuadTreeRenderableEventHandlerVisitor : public ISpatialTreeVisitor
{
public:
	QuadTreeRenderableEventHandlerVisitor(const FindQuery& findQuery, RenderBlockResultSet* target)
		: mFindQuery(findQuery), mResultSetTarget(target), mFoundResults(false)
	{
		assert_not_null(target);
	}

	virtual ~QuadTreeRenderableEventHandlerVisitor()
	{
	}

	inline bool HasFoundResults() const {
		return mFoundResults;
	}

// ISpatialTreeVisitor
public:
	virtual void Visit(SpatialSceneNode* item)
	{
		static_cast<Renderable*>(item)->PreRender(mFindQuery, mResultSetTarget);
		mFoundResults = true;
	}

private:
	const FindQuery& mFindQuery;
	RenderBlockResultSet* mResultSetTarget;
	bool mFoundResults;
};

bool QuadTreeSceneGroup::Find(const FindQuery& query, RenderBlockResultSet* _out_resultSet) const
{
	QuadTreeRenderableEventHandlerVisitor visitor(query, _out_resultSet);
	mQuadTree->Find(query.camera->GetViewFrustum(), &visitor);
	return visitor.HasFoundResults();
}

bool QuadTreeSceneGroup::Find(const FindQuery& query, LightSourceResultSet* _out_resultSet) const
{
	uint32 count = 0;
	const Frustum& frustum = query.camera->GetViewFrustum();
	auto lightSource = mLightSources.First();
	while (lightSource != nullptr) {
		auto next = lightSource->LightSourceLink.Tail;
		CollisionResult::Enum result = frustum.IsColliding(lightSource->GetBoundingBox());
		if (result != CollisionResult::OUTSIDE) {
			lightSource->CollectLightBlocks(query, _out_resultSet);
			count++;
		}
		lightSource = next;
	}

	return count > 0;
}
