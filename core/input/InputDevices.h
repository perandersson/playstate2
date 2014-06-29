#pragma once
#include "IInputDevices.h"

namespace core
{
	class InputDevices
	{
	public:
		static const IMouseDevice* GetMouseDevice(uint32 index);
		static std::vector<const IMouseDevice*> GetMouseDevices();
		static const IKeyboardDevice* GetKeyboardDevice(uint32 index);
		static std::vector<const IKeyboardDevice*> GetKeyboardDevices();
	};
}
