#pragma once
#include "../IInputDevices.h"
#include "Win32MouseDevice.h"
#include "Win32KeyboardDevice.h"

#include <Windows.h>

namespace core
{
	/*!
		\brief Microsoft Windows implementation of the IInputDevices interface
	*/
	class Win32InputDevices : public IInputDevices
	{
		typedef std::vector<std::shared_ptr<Win32MouseDevice>> MouseDevices;
		typedef std::vector<std::shared_ptr<Win32KeyboardDevice>> KeyboardDevices;

	public:
		Win32InputDevices();
		virtual ~Win32InputDevices();

		/*!
			\brief Process all the input devices connected to this computer
				
			\param activeWindow
						The game window
		*/
		void ProcessDevices(class Win32ActiveWindow* activeWindow);

	// IInputDevices
	public:
		virtual const IMouseDevice* GetMouseDevice(uint32 index);
		virtual std::vector<const IMouseDevice*> GetMouseDevices();
		virtual const IKeyboardDevice* GetKeyboardDevice(uint32 index);
		virtual std::vector<const IKeyboardDevice*> GetKeyboardDevices();

	private:
		MouseDevices mMouseDevices;
		KeyboardDevices mKeyboardDevices;
	};
}
