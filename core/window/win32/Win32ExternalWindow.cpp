#include "../../Memory.h"
#include "Win32ExternalWindow.h"
using namespace core;

Win32ExternalWindow::Win32ExternalWindow(IConfiguration* configuration)
: Win32GLActiveWindow(configuration)
{

}

Win32ExternalWindow::~Win32ExternalWindow()
{
}

void Win32ExternalWindow::Initialize(HWND windowHandle, const Size& size)
{
	mSize = size;
	Win32GLActiveWindow::InitializeDrivers(windowHandle, size);
}

bool Win32ExternalWindow::IsActive() const
{
	return true;
}

void Win32ExternalWindow::SetSize(const Size& size)
{
	mSize = size;
}

const Size& Win32ExternalWindow::GetSize() const
{
	return mSize;
}

void Win32ExternalWindow::SetTitle(const std::string& title)
{

}

const std::string& Win32ExternalWindow::GetTitle() const
{
	static const std::string title;
	return title;
}

void Win32ExternalWindow::AddWindowClosedListener(WindowClosedListener* listener)
{

}

void Win32ExternalWindow::RemoveWindowClosedListener(WindowClosedListener* listener)
{

}

void Win32ExternalWindow::AddWindowResizedListener(WindowResizedListener* listener)
{

}

void Win32ExternalWindow::RemoveWindowResizedListener(WindowResizedListener* listener)
{

}

void Win32ExternalWindow::SetFullscreen(bool fullscreen)
{
}

void Win32ExternalWindow::Alert(const std::string& title, const std::string& text, AlertType::Enum type)
{

}

void Win32ExternalWindow::Alert(const std::string& title, const Exception& e, AlertType::Enum type)
{

}
