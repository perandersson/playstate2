#include "../../Memory.h"
#include "Win32MouseDevice.h"
#include "../../window/win32/Win32ActiveWindow.h"
using namespace core;

Win32MouseDevice::Win32MouseDevice(uint32 index)
: mIndex(index), mEnabled(true)
{
}

Win32MouseDevice::~Win32MouseDevice()
{
}

void Win32MouseDevice::ProcessMouse(Win32ActiveWindow* activeWindow)
{
	if (!activeWindow->IsActive()) {
		mEnabled = false;
		return;
	}

	POINT p;
	if (GetCursorPos(&p)) {
		if (ScreenToClient(activeWindow->GetHandle(), &p)) {
			Point cursorPoint((int32)p.x, (int32)p.y);
			if (cursorPoint >= Point::ZERO && cursorPoint <= activeWindow->GetSize()) {
				mPosition = Point(p.x, p.y);
				mEnabled = true;
			}
			else {
				mEnabled = false;
			}
		}
	}
}

uint32 Win32MouseDevice::GetIndex() const
{
	return mIndex;
}

bool Win32MouseDevice::IsDown(MouseButton::Enum button) const
{
	if (!mEnabled)
		return false;

	return (GetKeyState(GetMouseButtonAsDWORD(button)) & 0x80) != 0;
}

bool Win32MouseDevice::IsUp(MouseButton::Enum button) const
{
	if (!mEnabled)
		return false;

	return (GetKeyState(GetMouseButtonAsDWORD(button)) & 0x80) == 0;
}

Point Win32MouseDevice::GetPosition() const
{
	return mPosition;
}

DWORD Win32MouseDevice::GetMouseButtonAsDWORD(MouseButton::Enum button)
{
	static const DWORD enums[MouseButton::SIZE] = {
		VK_LBUTTON,
		VK_MBUTTON,
		VK_RBUTTON
	};

	return enums[(uint32)button];
}
