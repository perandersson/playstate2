#include "../Memory.h"
#include "LuaStateScopedLock.h"
#include "IScriptManager.h"
using namespace core;

LuaStateScopedLock::LuaStateScopedLock(IScriptManager* scriptManager)
: mScriptManager(scriptManager), mLuaState(nullptr)
{

}

LuaStateScopedLock::~LuaStateScopedLock()
{
	if (mLuaState != nullptr) {
		mScriptManager->Unlock(mLuaState);
	}
}

lua_State* LuaStateScopedLock::GetLuaState()
{
	if (mLuaState == nullptr)
		mLuaState = mScriptManager->Lock();
	return mLuaState;
}
