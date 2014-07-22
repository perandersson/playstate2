#pragma once
#include "Win32GLActiveWindow.h"
#include <Windows.h>

namespace core
{
	class Win32ExternalWindow : public Win32GLActiveWindow
	{
	public:
		Win32ExternalWindow(IConfiguration* configuration);
		virtual ~Win32ExternalWindow();

	public:
		void Initialize(HWND windowHandle, const Size& size);
		
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
		Size mSize;
	};
}