#include "../Memory.h"
#include "SoundReceiver_script.h"
#include "../script/ScriptUtils.h"
using namespace core;

ScriptSoundReceiver::ScriptSoundReceiver(ISoundReceiver* soundReceiver)
: mSoundReceiver(soundReceiver)
{

}

ScriptSoundReceiver::~ScriptSoundReceiver()
{

}

void ScriptSoundReceiver::SetPosition(const Vector3& position)
{
	mSoundReceiver->SetPosition(position);
}

void ScriptSoundReceiver::LookAt(const Vector3& direction, const Vector3& up)
{
	mSoundReceiver->LookAt(direction, up);
}

//
// Script integration
//

int SoundReceiver_SetPosition(struct lua_State* L)
{
	const Vector3 position = ScriptUtils::ToVector3(L);
	ScriptSoundReceiver* self = ScriptUtils::ToObject<ScriptSoundReceiver>(L);
	if (self != nullptr)
		self->SetPosition(position);
	else
		ScriptUtils::LogError(L, "Expected: ScriptSoundReceiver.SetPosition(self, Vector3)");

	return 0;
}

int SoundReceiver_LookAt(struct lua_State* L)
{
	const Vector3 up = ScriptUtils::ToVector3(L);
	const Vector3 direction = ScriptUtils::ToVector3(L);
	ScriptSoundReceiver* self = ScriptUtils::ToObject<ScriptSoundReceiver>(L);
	if (self != nullptr)
		self->LookAt(direction, up);
	else
		ScriptUtils::LogError(L, "Expected: ScriptSoundReceiver.SetPosition(self, Vector3, Vector3)");

	return 0;
}

int SoundReceiver_GC(struct lua_State* L)
{
	ScriptSoundReceiver* self = ScriptUtils::ToObject<ScriptSoundReceiver>(L);
	if (self != nullptr) {
		delete self;
	}
	return 0;
}
