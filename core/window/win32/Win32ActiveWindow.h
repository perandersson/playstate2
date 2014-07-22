#pragma once
#include "Win32GLActiveWindow.h"
#include "../../input/win32/Win32InputDevices.h"
#include "../../configuration/IConfiguration.h"

namespace core
{
	class Win32ActiveWindow : public Win32GLActiveWindow
	{
	public:
		Win32ActiveWindow(HINSTANCE applicationHandle, IConfiguration* configuration, Win32InputDevices* inputDevices);
		virtual ~Win32ActiveWindow();

		//
		//
		void Initialize();

		virtual bool ProcessEvents();

		//
		// Process this windows internal events
		LRESULT CALLBACK ProcessEvent(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

		inline HWND GetHandle() const {
			return mWindowHandle;
		}

	public:
		virtual bool IsActive() const;
		virtual void SetSize(const Size& size);
		virtual const Size& GetSize() const;
		virtual void SetTitle(const std::string& title);
		virtual const std::string& GetTitle() const;
		virtual void AddWindowClosedListener(WindowClosedListener* listener);
		virtual void RemoveWindowClosedListener(WindowClosedListener* listener);
		virtual void AddWindowResizedListener(WindowResizedListener* listener);
		virtual void RemoveWindowResizedListener(WindowResizedListener* listener);
		virtual void SetFullscreen(bool fullscreen);
		virtual void Alert(const std::string& title, const std::string& text, AlertType::Enum type);
		virtual void Alert(const std::string& title, const Exception& e, AlertType::Enum type);

	private:
		IConfiguration* mConfiguration;
		Win32InputDevices* mInputDevices;

		HINSTANCE mApplicationHandle;
		HWND mWindowHandle;
		MSG mMessageQueue;

		LinkedList<WindowResizedListener> mWindowResizeListeners;
		LinkedList<WindowClosedListener> mWindowClosedListeners;

		std::string mTitle;

		DWORD mStyle;
		DWORD mExStyle;

		Size mSize;
		Size mPrevSize;

		bool mResizingWindow;
		bool mFullscreen;
	};
}