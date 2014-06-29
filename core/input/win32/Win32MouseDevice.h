#pragma once
#include "../IMouseDevice.h"
#include <Windows.h>

namespace core
{
	/*!
		\brief Microsoft Windows implementation of the IMouseDevice interface
	*/
	class Win32MouseDevice : public IMouseDevice
	{
	public:
		Win32MouseDevice(uint32 index);
		virtual ~Win32MouseDevice();

		/*!
			\brief Process the internal mouse data

			\param activeWindow
						The active game window. Useful to know if the window is active and if the mouse is 
						actually inside the window or not.
		*/
		void ProcessMouse(class Win32ActiveWindow* activeWindow);

	// IMouseDevice
	public:
		virtual uint32 GetIndex() const;
		virtual bool IsDown(MouseButton::Enum button) const;
		virtual bool IsUp(MouseButton::Enum button) const;
		virtual Point GetPosition() const;

	private:
		static DWORD GetMouseButtonAsDWORD(MouseButton::Enum button);

	private:
		uint32 mIndex;
		bool mEnabled;
		Point mPosition;
	};
}
