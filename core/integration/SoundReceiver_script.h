#pragma once
#include "../sound/ISoundReceiver.h"
#include "../script/ScriptObject.h"

namespace core
{
	class ScriptSoundReceiver : public ScriptObject, public ISoundReceiver
	{
		DECLARE_SCRIPT_OBJECT(SoundReceiver);

	public:
		ScriptSoundReceiver(ISoundReceiver* soundReceiver);
		virtual ~ScriptSoundReceiver();

	// ISoundReceiver
	public:
		virtual void SetPosition(const Vector3& position);
		virtual void LookAt(const Vector3& direction, const Vector3& up);

	private:
		ISoundReceiver* mSoundReceiver;
	};
}

//
// Script integration
//

int SoundReceiver_SetPosition(struct lua_State* L);
int SoundReceiver_LookAt(struct lua_State* L);
int SoundReceiver_GC(struct lua_State* L);

static luaL_Reg SoundReceiver_Methods[] = {
	{ "SetPosition", SoundReceiver_SetPosition },
	{ "LookAt", SoundReceiver_LookAt },
	{ SCRIPT_GC, SoundReceiver_GC },
	{ nullptr, nullptr }
};
