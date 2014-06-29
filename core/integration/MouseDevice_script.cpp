#include "../Memory.h"
#include "MouseDevice_script.h"
#include "../input/InputDevices.h"
using namespace core;

ScriptMouseDevice::ScriptMouseDevice(const IMouseDevice* mouseDevice)
: ScriptObject(), mMouseDevice(mouseDevice)
{

}

ScriptMouseDevice::~ScriptMouseDevice()
{

}

uint32 ScriptMouseDevice::GetIndex() const
{
	return mMouseDevice->GetIndex();
}

bool ScriptMouseDevice::IsDown(MouseButton::Enum button) const
{
	return mMouseDevice->IsDown(button);
}

bool ScriptMouseDevice::IsUp(MouseButton::Enum button) const
{
	return mMouseDevice->IsUp(button);
}

Point ScriptMouseDevice::GetPosition() const
{
	return mMouseDevice->GetPosition();
}

int MouseDevice_GC(struct lua_State* L)
{
	const ScriptMouseDevice* self = ScriptUtils::ToObject<ScriptMouseDevice>(L);
	if (self != nullptr)
		delete self;

	return 0;
}

int MouseDevice_GetIndex(struct lua_State* L)
{
	const ScriptMouseDevice* self = ScriptUtils::ToObject<ScriptMouseDevice>(L);
	if (self != nullptr)
		return ScriptUtils::PushUInt32(L, self->GetIndex());
	else
		ScriptUtils::LogError(L, "Expected: ScriptMouseDevice.GetIndex(self)");

	return ScriptUtils::PushUInt32(L, 0);
}

int MouseDevice_IsUp(struct lua_State* L)
{
	const MouseButton::Enum button = ScriptUtils::ToEnum<MouseButton::Enum>(L);
	const ScriptMouseDevice* self = ScriptUtils::ToObject<ScriptMouseDevice>(L);
	if (self != nullptr)
		return ScriptUtils::PushBool(L, self->IsUp(button));
	else
		ScriptUtils::LogError(L, "Expected: ScriptMouseDevice.IsUp(self, MouseButton)");

	return ScriptUtils::PushBool(L, false);
}

int MouseDevice_IsDown(struct lua_State* L)
{
	const MouseButton::Enum button = ScriptUtils::ToEnum<MouseButton::Enum>(L);
	const ScriptMouseDevice* self = ScriptUtils::ToObject<ScriptMouseDevice>(L);
	if (self != nullptr)
		return ScriptUtils::PushBool(L, self->IsDown(button));
	else
		ScriptUtils::LogError(L, "Expected: ScriptMouseDevice.IsDown(self, MouseButton)");

	return ScriptUtils::PushBool(L, false);
}

int MouseDevice_GetPosition(struct lua_State* L)
{
	const ScriptMouseDevice* self = ScriptUtils::ToObject<ScriptMouseDevice>(L);
	if (self != nullptr)
		return ScriptUtils::PushPoint(L, self->GetPosition());
	else
		ScriptUtils::LogError(L, "Expected: ScriptMouseDevice.GetPosition(self)");

	return ScriptUtils::PushPoint(L, Point::ZERO);
}
