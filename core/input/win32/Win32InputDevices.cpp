#include "../../Memory.h"
#include "Win32InputDevices.h"
using namespace core;

Win32InputDevices::Win32InputDevices()
{
	mMouseDevices.push_back(std::shared_ptr<Win32MouseDevice>(new Win32MouseDevice(0)));
	mKeyboardDevices.push_back(std::shared_ptr<Win32KeyboardDevice>(new Win32KeyboardDevice(0)));
}

Win32InputDevices::~Win32InputDevices()
{
}

void Win32InputDevices::ProcessDevices(Win32ActiveWindow* window)
{
	MouseDevices::size_type numMouseDevices = mMouseDevices.size();
	for (MouseDevices::size_type i = 0; i < numMouseDevices; ++i) {
		mMouseDevices[i]->ProcessMouse(window);
	}

	KeyboardDevices::size_type numKeyboardDevices = mKeyboardDevices.size();
	for (KeyboardDevices::size_type i = 0; i < numKeyboardDevices; ++i) {
		mKeyboardDevices[i]->ProcessKeyboard(window);
	}
}

const IMouseDevice* Win32InputDevices::GetMouseDevice(uint32 index)
{
	if (mMouseDevices.size() < index)
		return nullptr;

	return mMouseDevices[index].get();
}

std::vector<const IMouseDevice*> Win32InputDevices::GetMouseDevices()
{
	std::vector<const IMouseDevice*> mouseDevices;
	for (uint32 i = 0; i < mMouseDevices.size(); ++i) {
		mouseDevices.push_back(mMouseDevices[i].get());
	}
	return mouseDevices;
}

const IKeyboardDevice* Win32InputDevices::GetKeyboardDevice(uint32 index)
{
	if (mKeyboardDevices.size() < index)
		return nullptr;

	return mKeyboardDevices[index].get();
}

std::vector<const IKeyboardDevice*> Win32InputDevices::GetKeyboardDevices()
{
	std::vector<const IKeyboardDevice*> keyboardDevices;
	for (uint32 i = 0; i < mKeyboardDevices.size(); ++i) {
		keyboardDevices.push_back(mKeyboardDevices[i].get());
	}
	return keyboardDevices;
}
