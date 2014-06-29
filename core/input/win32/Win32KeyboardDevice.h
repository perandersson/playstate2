#pragma once
#include "../IKeyboardDevice.h"
#include <Windows.h>

namespace core
{
	/*!
		\brief Microsoft Windows implementation of the IKeyboardDevice interface
	*/
	class Win32KeyboardDevice : public IKeyboardDevice
	{
	public:
		Win32KeyboardDevice(uint32 index);
		virtual ~Win32KeyboardDevice();
			
		/*!
			\brief Process the internal keyboard data
				
			\param activeWindow
						The active game window. Useful to know if the window is active or not.
		*/
		void ProcessKeyboard(class Win32ActiveWindow* activeWindow);

	// IKeyboardDevice
	public:
		virtual uint32 GetIndex() const;
		virtual bool IsDown(KeyboardKey::Enum key) const;
		virtual bool IsUp(KeyboardKey::Enum key) const;

	private:
		static DWORD GetKeyboardKeyAsDWORD(KeyboardKey::Enum button);

	private:
		uint32 mIndex;
		bool mEnabled;
	};
}
