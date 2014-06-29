#include "../../Memory.h"
#include "Win32KeyboardDevice.h"
#include "../../window/win32/Win32ActiveWindow.h"
using namespace core;

Win32KeyboardDevice::Win32KeyboardDevice(uint32 index)
: mIndex(index), mEnabled(false)
{
}

Win32KeyboardDevice::~Win32KeyboardDevice()
{
}

void Win32KeyboardDevice::ProcessKeyboard(Win32ActiveWindow* activeWindow)
{
	mEnabled = activeWindow->IsActive();
}

uint32 Win32KeyboardDevice::GetIndex() const
{
	return mIndex;
}

bool Win32KeyboardDevice::IsDown(KeyboardKey::Enum button) const
{
	if (!mEnabled)
		return false;

	return (GetKeyState(GetKeyboardKeyAsDWORD(button)) & 0x80) != 0;
}

bool Win32KeyboardDevice::IsUp(KeyboardKey::Enum button) const
{
	if (!mEnabled)
		return false;

	return (GetKeyState(GetKeyboardKeyAsDWORD(button)) & 0x80) == 0;
}

DWORD Win32KeyboardDevice::GetKeyboardKeyAsDWORD(KeyboardKey::Enum button)
{
	static const DWORD enums[KeyboardKey::SIZE] = {
		VK_SPACE, VK_ESCAPE, VK_BACK, VK_RETURN, VK_INSERT, VK_DELETE, VK_HOME, VK_END,
		VK_PRIOR, VK_NEXT, VK_TAB,
		VK_LCONTROL, VK_RCONTROL,
		VK_LSHIFT, VK_RSHIFT,
		VK_LMENU, VK_RMENU,
		VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN,
		(int)('A'), (int)('B'), (int)('C'), (int)('D'), (int)('E'), (int)('F'), (int)('G'), (int)('H'), (int)('I'), (int)('J'), (int)('K'), (int)('L'), (int)('M'), (int)('N'), (int)('O'),
		(int)('P'), (int)('Q'), (int)('R'), (int)('S'), (int)('T'), (int)('U'), (int)('V'), (int)('W'), (int)('X'), (int)('Y'), (int)('Z'),
		(int)('0'), (int)('1'), (int)('2'), (int)('3'), (int)('4'), (int)('5'), (int)('6'), (int)('7'), (int)('8'), (int)('9')
	};

	return enums[(uint32)button];
}
