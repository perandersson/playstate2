#include "../../Memory.h"
#include "LightSource.h"
#include "../SceneGroup.h"
#include <atomic>
using namespace core;

namespace {
	static std::atomic<uint32> uid(0);
	uint32 GenUID() {
		return ++uid;
	}
}

LightSource::LightSource()
: SpatialSceneNode(), mUID(GenUID())
{
}

LightSource::~LightSource()
{
}

void LightSource::OnAddedToSceneGroup()
{
	SceneGroup* group = GetSceneGroup();
	group->AddLightSource(this);
}

void LightSource::OnRemovedFromSceneGroup()
{
	SceneGroup* group = GetSceneGroup();
	group->RemoveLightSource(this);
}
