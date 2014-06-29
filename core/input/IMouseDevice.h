#pragma once
#include "../typedefs.h"
#include "../math/point.h"
#include "MouseButton.h"

namespace core
{
	/*!
		\brief The IMouseDevice interface is used to access a mouse connected to the computer.

		You are able to query the computer for a mouse input device by using the IInputDevice service.
		Example:

		{@code
			const IMouseDevice* device = InputDevices::GetMouseDevice(0);
			if(device->IsDown(MouseButton::LEFT)) {
				// Do stuff!
			}
		}
	*/
	class IMouseDevice
	{
	public:
		virtual ~IMouseDevice() {}

		/*!
			\brief Retrieves the mouse's device index. 

			This method is useful to identify this mouse if more then one mouse is connected to the same comupter.
	
			\return The mouse device index
		*/
		virtual uint32 GetIndex() const = 0;

		/*!
			\brief Check if a mouse button is pressed down.

			\param button
						The button we want to check if it's pressed down.
			\return TRUE if the supplied button is pressed down; FALSE otherwise.
		*/
		virtual bool IsDown(MouseButton::Enum button) const = 0;
		
		/*!
			\brief Check if a mouse button is not pressed down.

			\param button
						The button we want to check if it's not pressed down.
			\return TRUE if the supplied button is not pressed down; FALSE otherwise.
		*/
		virtual bool IsUp(MouseButton::Enum button) const = 0;

		/*!
			\brief Retrieves the position for this mouse device

			\return The mouse position
		*/
		virtual Point GetPosition() const = 0;
	};
}
