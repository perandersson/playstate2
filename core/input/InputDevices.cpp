#include "../Memory.h"
#include "InputDevices.h"
#include "../kernel/Kernel.h"
using namespace core;

const IMouseDevice* InputDevices::GetMouseDevice(uint32 index)
{
	return Kernel::GetInputDevices()->GetMouseDevice(index);
}

std::vector<const IMouseDevice*> InputDevices::GetMouseDevices()
{
	return Kernel::GetInputDevices()->GetMouseDevices();
}

const IKeyboardDevice* InputDevices::GetKeyboardDevice(uint32 index)
{
	return Kernel::GetInputDevices()->GetKeyboardDevice(index);
}

std::vector<const IKeyboardDevice*> InputDevices::GetKeyboardDevices()
{
	return Kernel::GetInputDevices()->GetKeyboardDevices();
}
