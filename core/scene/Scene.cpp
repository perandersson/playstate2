#include "../Memory.h"
#include "Scene.h"
#include "Updatable.h"
#include "Tickable.h"
#include "../script/ScriptUtils.h"
#include "../game/Game.h"
#include "../Singleton.h"
#include "rendering/DefaultRenderBlockResultSetSorter.h"
#include <algorithm>
using namespace core;

Scene::Scene()
: mSceneGroups(offsetof(SceneGroup, SceneLink)), mTimeSinceLastTick(0), mActiveCamera(nullptr), mAmbientLight(Color::WHITE),
mSceneGroupListeners(offsetof(SceneGroupListener, SceneGroupListenerLink))
{
}

Scene::~Scene()
{
	mSceneGroups.DeleteAll();
	mSceneGroupListeners.DeleteAll();
}

void Scene::AddSceneGroup(SceneGroup* group)
{
	assert_not_null(group);
	mSceneGroups.AddLast(group);
	group->AttachedToScene();

	auto listener = mSceneGroupListeners.First();
	while (listener != nullptr) {
		auto next = listener->SceneGroupListenerLink.Tail;
		listener->OnSceneGroupAdded(group);
		listener = next;
	}
}

void Scene::RemoveSceneGroup(SceneGroup* group)
{
	assert_not_null(group);
	mSceneGroups.Remove(group);
	group->DetachedFromScene();

	auto listener = mSceneGroupListeners.First();
	while (listener != nullptr) {
		auto next = listener->SceneGroupListenerLink.Tail;
		listener->OnSceneGroupRemoved(group);
		listener = next;
	}
}

void Scene::AddSceneGroupListener(SceneGroupListener* listener)
{
	mSceneGroupListeners.AddLast(listener);
}

void Scene::RemoveSceneGroupListener(SceneGroupListener* listener)
{
	mSceneGroupListeners.Remove(listener);
}

void Scene::Update()
{
	auto group = mSceneGroups.First();
	while (group != NULL) {
		auto next = group->SceneLink.Tail;
		group->UpdateUpdatables();
		group = next;
	}

	mTimeSinceLastTick += GameDeltaTime;
	static const float32 ApproxSecondsPerTick = (1.0f / (float32)TicksPerSecond);
	if (mTimeSinceLastTick > ApproxSecondsPerTick) {
		GameTickDeltaTime = mTimeSinceLastTick;
		group = mSceneGroups.First();
		while (group != NULL) {
			auto next = group->SceneLink.Tail;
			group->UpdateTickables();
			group = next;
		}
		mTimeSinceLastTick = 0;
	}
}

void Scene::FireEvent(const Event* evt)
{
	auto group = mSceneGroups.First();
	while (group != NULL) {
		auto next = group->SceneLink.Tail;
		group->FireEvent(evt);
		group = next;
	}
}

void Scene::FireEvent(const Event* evt, typemask typeMask)
{
	auto group = mSceneGroups.First();
	while (group != NULL) {
		auto next = group->SceneLink.Tail;
		group->FireEvent(evt, typeMask);
		group = next;
	}
}

void Scene::SetActiveCamera(Camera* camera)
{
	mActiveCamera = camera;
}

void Scene::SetAmbientLight(const Color& color)
{
	mAmbientLight = color;
}

bool Scene::Find(const FindQuery& query, RenderBlockResultSet* _out_resultSet) const
{
	DefaultRenderBlockResultSetSorter sorter;
	return Find(query, &sorter, _out_resultSet);
}

bool Scene::Find(const FindQuery& query, IRenderBlockSorter* sorter, RenderBlockResultSet* _out_resultSet) const
{
	assert_not_null(_out_resultSet);
	_out_resultSet->Reset();

	bool found = false;
	const SceneGroup* group = mSceneGroups.First();
	while (group != NULL) {
		const SceneGroup* next = group->SceneLink.Tail;
		if (group->Find(query, _out_resultSet))
			found = true;
		group = next;
	}
	if (found)
		_out_resultSet->Sort(sorter);
	return found;
}

bool Scene::Find(const FindQuery& query, LightSourceResultSet* _out_resultSet) const
{
	assert_not_null(_out_resultSet);
	_out_resultSet->Reset();

	bool found = false;
	const SceneGroup* group = mSceneGroups.First();
	while (group != NULL) {
		const SceneGroup* next = group->SceneLink.Tail;
		if (group->Find(query, _out_resultSet))
			found = true;
		group = next;
	}
	return found;
}
