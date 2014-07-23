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
: SpatialSceneNode(), mUID(GenUID()), mStaticShadowCaster(true), mDynamicShadowCaster(false)
{
}

LightSource::~LightSource()
{
}

void LightSource::SetStaticShadowCaster(bool b)
{
	mStaticShadowCaster = b;
}


void LightSource::SetDynamicShadowCaster(bool b)
{
	mDynamicShadowCaster = b;
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
