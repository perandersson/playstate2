#pragma once
#include "IMouseDevice.h"
#include "IKeyboardDevice.h"
#include <vector>

namespace core
{
	/*!
		\brief The IInputDevices interface is used to access the computer's available input devices, such as the keyboard and the mouse.

		IInputDevices is a service interface, which means that you can access it through the InputDevices class or by using:
		{@code
			IInputDevices* inputDevices = Kernel::GetInputDevices();
		}

		Example:
		{@code
			// Using the service interface directly
			IInputDevices* inputDevices = Kernel::GetInputDevices();
			const IMouseDevice* mouse = inputDevices->GetMouseDevice(0);
			Point position = mouse->GetPosition();

			// Using the helper class
			const IMouseDevice* mouse = InputDevices::GetMouseDevice(0);
			Point position = mouse->GetPosition();			
		}
	*/
	class IInputDevices
	{
	public:
		virtual ~IInputDevices() {}

	public:
		/*!
			\brief Retrieves the mouse device on the given index

			\param index
						The mouse device index
			\return A pointer to the mouse device; NULL if no mouse is found.
		*/
		virtual const IMouseDevice* GetMouseDevice(uint32 index) = 0;
		
		/*!
			\brief Retrieves a list of all the mouse devices connected to this computer

			\return A std::vector containing the mouse devices
		*/
		virtual std::vector<const IMouseDevice*> GetMouseDevices() = 0;
		
		/*!
			\brief Retrieves the keyboard device on the given index

			\param index
						The keyboard device index
			\return A pointer to the keyboard device; NULL if no keyboard is found.
		*/
		virtual const IKeyboardDevice* GetKeyboardDevice(uint32 index) = 0;
		
		/*!
			\brief Retrieves a list of all the keyboard devices connected to this computer

			\return A std::vector containing the keyboard devices
		*/
		virtual std::vector<const IKeyboardDevice*> GetKeyboardDevices() = 0;
	};

}
