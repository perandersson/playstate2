#include "../Memory.h"
#include "LinkedListSceneGroup.h"
#include "../script/ScriptUtils.h"
#include "light/LightSource.h"
#include "Updatable.h"
#include "Tickable.h"
#include "rendering/Renderable.h"
using namespace core;

LinkedListSceneGroup::LinkedListSceneGroup()
: SceneGroup(), mRenderables(offsetof(Renderable, RenderableLink)), mLightSources(offsetof(LightSource, LightSourceLink)),
mUpdatables(offsetof(Updatable, UpdatableLink)), mTickables(offsetof(Tickable, TickableLink))
{

}

LinkedListSceneGroup::~LinkedListSceneGroup()
{
	mTickables.DeleteAll();
	mUpdatables.DeleteAll();
	mRenderables.DeleteAll();
	mLightSources.DeleteAll();
}

void LinkedListSceneGroup::AddUpdatable(Updatable* updatable)
{
	assert_not_null(updatable);
	mUpdatables.AddLast(updatable);
}

void LinkedListSceneGroup::RemoveUpdatable(Updatable* updatable)
{
	assert_not_null(updatable);
	mUpdatables.Remove(updatable);
}

void LinkedListSceneGroup::AddTickable(Tickable* tickable)
{
	assert_not_null(tickable);
	mTickables.AddLast(tickable);
}

void LinkedListSceneGroup::RemoveTickable(Tickable* tickable)
{
	assert_not_null(tickable);
	mTickables.Remove(tickable);
}

void LinkedListSceneGroup::AddRenderable(Renderable* renderable)
{
	assert_not_null(renderable);
	mRenderables.AddLast(renderable);
}

void LinkedListSceneGroup::RemoveRenderable(Renderable* renderable)
{
	assert_not_null(renderable);
	mRenderables.Remove(renderable);
}

void LinkedListSceneGroup::AddLightSource(LightSource* lightSource)
{
	assert_not_null(lightSource);
	mLightSources.AddLast(lightSource);
}

void LinkedListSceneGroup::RemoveLightSource(LightSource* lightSource)
{
	assert_not_null(lightSource);
	mLightSources.Remove(lightSource);
}

void LinkedListSceneGroup::UpdateUpdatables()
{
	auto updatable = mUpdatables.First();
	while (updatable != nullptr) {
		auto next = updatable->UpdatableLink.Tail;
		updatable->Update();
		updatable = next;
	}
}

void LinkedListSceneGroup::UpdateTickables()
{
	auto tickable = mTickables.First();
	while (tickable != nullptr) {
		auto next = tickable->TickableLink.Tail;
		tickable->Tick();
		tickable = next;
	}
}

bool LinkedListSceneGroup::Find(const FindQuery& query, RenderBlockResultSet* _out_resultSet) const
{
	uint32 count = 0;
	const Frustum* frustum = query.frustum;
	auto renderable = mRenderables.First();
	while (renderable != nullptr) {
		auto next = renderable->RenderableLink.Tail;
		CollisionResult::Enum result = frustum->IsColliding(renderable->GetBoundingBox());
		if (result != CollisionResult::OUTSIDE) {
			renderable->PreRender(query, _out_resultSet);
			count++;
		}
		renderable = next;
	}

	return count > 0;
}

bool LinkedListSceneGroup::Find(const FindQuery& query, LightSourceResultSet* _out_resultSet) const
{
	uint32 count = 0;
	const Frustum* frustum = query.frustum;
	auto lightSource = mLightSources.First();
	while (lightSource != nullptr) {
		auto next = lightSource->LightSourceLink.Tail;
		CollisionResult::Enum result = frustum->IsColliding(lightSource->GetBoundingBox());
		if (result != CollisionResult::OUTSIDE) {
			lightSource->CollectLightBlocks(query, _out_resultSet);
			count++;
		}
		lightSource = next;
	}

	return count > 0;
}
