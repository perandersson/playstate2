#pragma once
#include "../typedefs.h"
#include "KeyboardKey.h"

namespace core
{
	/*!
		\brief The IKeyboardDevice interface is used to access a keyboard connected to the computer.

		You are able to query the computer for a keyboard input device by using the IInputDevice service.
		Example:

		{@code
			const IKeyboardDevice* device = InputDevices::GetKeyboardDevice(0);
			if(device->IsDown(KeyboardKey::RIGHT_ALT)) {
				// Do stuff!
			}
		}
	*/
	class IKeyboardDevice
	{
	public:
		virtual ~IKeyboardDevice() {}

		/*!
			\brief Retrieves the keyboard's device index. 

			This method is useful to identify this keyboard if more then one keyboard is connected to the same comupter.
	
			\return The keyboard device index
		*/
		virtual uint32 GetIndex() const = 0;

		/*!
			\brief Check if a keyboard key is pressed down.

			\param key
						The key we want to check if it's pressed down.
			\return TRUE if the supplied key is pressed down; FALSE otherwise.
		*/
		virtual bool IsDown(KeyboardKey::Enum key) const = 0;
		
		/*!
			\brief Check if a keyboard key is not pressed down.

			\param key
						The key we want to check if it's not pressed down.
			\return TRUE if the supplied key is not pressed down; FALSE otherwise.
		*/
		virtual bool IsUp(KeyboardKey::Enum key) const = 0;
	};
}
