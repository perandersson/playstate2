#include "../../Memory.h"
#include "Win32ActiveWindow.h"
#include <algorithm>
using namespace core;

// Anonymouse code
namespace {
	Win32ActiveWindow* _window = NULL;
	LRESULT CALLBACK WindowsWindowWindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
	{
		return _window->ProcessEvent(hwnd, message, wparam, lparam);
	}
}

static const char* PLAYSTATE_CLASS_NAME = "playstate";

Win32ActiveWindow::Win32ActiveWindow(HINSTANCE applicationHandle, IConfiguration* configuration, Win32InputDevices* inputDevices)
: Win32GLActiveWindow(configuration), mInputDevices(inputDevices), mConfiguration(configuration), mApplicationHandle(applicationHandle), mWindowHandle(NULL), mResizingWindow(false),
mFullscreen(false)
{
	_window = this;
	memset(&mMessageQueue, 0, sizeof(MSG));
	// , mTitle(title), mSize(size), mPrevSize(size)
}

Win32ActiveWindow::~Win32ActiveWindow()
{
	mWindowResizeListeners.clear();
	mWindowClosedListeners.clear();

	Win32GLActiveWindow::ReleaseDrivers();

	if (mFullscreen) {
		ChangeDisplaySettings(NULL, 0);
		//ShowCursor(TRUE);
		mFullscreen = false;
	}

	if (mWindowHandle != NULL) {
		CloseWindow(mWindowHandle);
		mWindowHandle = NULL;
	}

	UnregisterClass(PLAYSTATE_CLASS_NAME, mApplicationHandle); 
	mApplicationHandle = NULL;
}

void Win32ActiveWindow::Initialize()
{
	mSize = mPrevSize = mConfiguration->ToSize("window.size", Size(1024, 768));
	mTitle = mConfiguration->ToString("window.title");

	// Register the class used by the 
	WNDCLASSEX windowProperties = { 0 };
	windowProperties.cbSize = sizeof(WNDCLASSEX);
	windowProperties.style = CS_HREDRAW | CS_VREDRAW;
	windowProperties.lpfnWndProc = WindowsWindowWindowProc;
	windowProperties.hInstance = mApplicationHandle;
	windowProperties.lpszClassName = PLAYSTATE_CLASS_NAME;
	windowProperties.hCursor = (HCURSOR)LoadImage(NULL, MAKEINTRESOURCE(IDC_ARROW), IMAGE_CURSOR, 0, 0, LR_SHARED);
	RegisterClassEx(&windowProperties);

	mFullscreen = mConfiguration->ToBool("window.fullscreen", true);
	if (mFullscreen) {
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = mSize.width;
		dmScreenSettings.dmPelsHeight = mSize.height;
		dmScreenSettings.dmBitsPerPel = mConfiguration->ToInt32("window.bpp", 32);
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			mFullscreen = false;
			assert_not_implemented();
		}
		else {
			// Invoke all the listener classes. 
		}
	}

	if (mFullscreen) {
		mExStyle = WS_EX_APPWINDOW;
		mStyle = WS_POPUP;
		//ShowCursor(FALSE);
	}
	else {
		mExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		mStyle = WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_OVERLAPPEDWINDOW;
	}

	RECT windowSize = { 0, 0, (LONG)mSize.x, (LONG)mSize.y };
	AdjustWindowRectEx(&windowSize, mStyle, false, mExStyle);

	mWindowHandle = CreateWindowEx(mExStyle, PLAYSTATE_CLASS_NAME, mTitle.c_str(), mStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0,
		windowSize.right - windowSize.left, windowSize.bottom - windowSize.top,
		NULL, NULL, mApplicationHandle, NULL);

	if (mWindowHandle != NULL) {
		ShowWindow(mWindowHandle, SW_SHOW);
		SetForegroundWindow(mWindowHandle);
		SetFocus(mWindowHandle);
		UpdateWindow(mWindowHandle);
	}

	Win32GLActiveWindow::InitializeDrivers(mWindowHandle, mSize);

	SetSize(mSize);
}

const Size& Win32ActiveWindow::GetSize() const
{
	return mSize;
}

void Win32ActiveWindow::SetSize(const Size& size)
{
	// Prevent the size from being smaler than 1
	const int32 width = size.x > 0 ? size.x : 1;
	const int32 height = size.y > 0 ? size.y : 1;

	// Refresh the DX render targets with the new size
	mSize = Size(width, height);

	// Resize the fullscreen
	if (mFullscreen) {
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = mSize.width;
		dmScreenSettings.dmPelsHeight = mSize.height;
		dmScreenSettings.dmBitsPerPel = mConfiguration->ToInt32("window.bpp", 32);
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			mFullscreen = false;
			assert_not_implemented();
		}
		else {
			// Invoke all the listener classes. 
		}
	}

	// Make sure that the size of the window takes the actual window borders into account
	RECT windowRect = { 0, 0, (LONG)mSize.x, (LONG)mSize.y };
	AdjustWindowRectEx(&windowRect, mStyle, FALSE, mExStyle);

	// Update window size
	SetWindowPos(mWindowHandle, NULL, 0, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, SWP_NOMOVE);

	// Notify listeners
	WindowResizedListeners::size_type numListeners = mWindowResizeListeners.size();
	for (WindowResizedListeners::size_type i = 0; i < numListeners; ++i) {
		mWindowResizeListeners[i]->OnWindowResized(mSize);
	}

	mPrevSize = mSize;
}

const std::string& Win32ActiveWindow::GetTitle() const
{
	return mTitle;
}

void Win32ActiveWindow::SetTitle(const std::string& title)
{
	mTitle = title;
	SetWindowText(mWindowHandle, title.c_str());
}

bool Win32ActiveWindow::IsActive() const
{
	HWND focusedWindowHandle = GetFocus();
	return mWindowHandle == focusedWindowHandle;
}

LRESULT CALLBACK Win32ActiveWindow::ProcessEvent(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case WM_ACTIVATE:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	//case WM_SYSCOMMAND:
	//{
	//	// Prevent screen-saver
	//	switch (wparam) 
	//	{
	//	case SC_SCREENSAVE:
	//	case SC_MONITORPOWER:
	//		return 0;
	//	}
	//	break;
	//}
	case WM_ENTERSIZEMOVE:
		mResizingWindow = true;
		break;
	case WM_SIZE:
		if (mResizingWindow) return DefWindowProc(hwnd, message, wparam, lparam);
	case WM_EXITSIZEMOVE:
	{
		mResizingWindow = false;
		RECT rect;
		if (!GetClientRect(mWindowHandle, &rect))
			break;

		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		// Prevent the width and height to be 0 or smaller.
		width = width <= 0 ? 1 : width;
		height = height <= 0 ? 1 : height;

		// Refresh the DX render targets with the new size
		mSize = Size(width, height);

		break;
	}
	default:
		return DefWindowProc(hwnd, message, wparam, lparam);
	}

	return 0;
}

bool Win32ActiveWindow::ProcessEvents()
{
	while (PeekMessage(&mMessageQueue, NULL, 0, 0, PM_REMOVE)) {
		if (mMessageQueue.message == WM_QUIT) {
			WindowClosedListeners::size_type size = mWindowClosedListeners.size();
			for (WindowClosedListeners::size_type i = 0; i < size; ++i) {
				mWindowClosedListeners[i]->OnWindowClosing();
			}

			return false;;
		}

		TranslateMessage(&mMessageQueue);
		DispatchMessage(&mMessageQueue);
	}

	// Dispatch resize events if a resize has occured.
	if (mSize != mPrevSize) {
		mPrevSize = mSize;
		WindowResizedListeners::size_type size = mWindowResizeListeners.size();
		for (WindowResizedListeners::size_type i = 0; i < size; ++i) {
			mWindowResizeListeners[i]->OnWindowResized(mSize);
		}
	}

	mInputDevices->ProcessDevices(this);	
	return Win32GLActiveWindow::ProcessEvents();
}


void Win32ActiveWindow::AddWindowResizedListener(IWindowResizedListener* listener)
{
	mWindowResizeListeners.push_back(listener);
}

void Win32ActiveWindow::RemoveWindowResizedListener(IWindowResizedListener* listener)
{
	WindowResizedListeners::iterator it = std::find(mWindowResizeListeners.begin(), mWindowResizeListeners.end(), listener);
	if (it != mWindowResizeListeners.end())
		mWindowResizeListeners.erase(it);
}

void Win32ActiveWindow::AddWindowClosedListener(IWindowClosedListener* listener)
{
	mWindowClosedListeners.push_back(listener);
}

void Win32ActiveWindow::RemoveWindowClosedListener(IWindowClosedListener* listener)
{
	WindowClosedListeners::iterator it = std::find(mWindowClosedListeners.begin(), mWindowClosedListeners.end(), listener);
	if (it != mWindowClosedListeners.end())
		mWindowClosedListeners.erase(it);
}

void Win32ActiveWindow::SetFullscreen(bool fullscreen)
{
	assert_not_implemented();
	if (mFullscreen == fullscreen)
		return;

	// TODO: Change fullscreen property

	mFullscreen = fullscreen;
}

void Win32ActiveWindow::Alert(const std::string& title, const std::string& text, AlertType::Enum type)
{
	static const UINT types[3] = {
		MB_ICONEXCLAMATION,
		MB_ICONWARNING,
		MB_ICONERROR
	};

	MessageBox(NULL, text.c_str(), title.c_str(), MB_OK | types[(uint32)type]);
}

void Win32ActiveWindow::Alert(const std::string& title, const Exception& e, AlertType::Enum type)
{
#undef GetMessage
	Win32ActiveWindow::Alert(title, e.GetMessage(), type);
}
