#include "../../Memory.h"
#include "LightSource.h"
#include "../SceneGroup.h"
using namespace core;

LightSource::LightSource()
: SpatialSceneNode()
{
}

LightSource::~LightSource()
{
}

void LightSource::OnAddedToSceneGroup()
{
	SceneGroup* group = GetGroup();
	group->AddLightSource(this);
}

void LightSource::OnRemovedFromSceneGroup()
{
	SceneGroup* group = GetGroup();
	group->RemoveLightSource(this);
}
