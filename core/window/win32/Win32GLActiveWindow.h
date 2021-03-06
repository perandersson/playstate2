#pragma once
#include "../IActiveWindow.h"
#include "../../configuration/IConfiguration.h"
#include "../../rendering/win32/Win32RenderContext.h"

#include <Windows.h>
#include "../../rendering/GLEWMX.h"

namespace core
{
	class Win32GLActiveWindow : public IActiveWindow
	{
	protected:
		Win32GLActiveWindow(IConfiguration* configuration);

		//
		// Initialize the graphics drivers
		//
		void InitializeDrivers(HWND windowHandle, const Size& size);

		//
		// Release the graphics drivers
		//
		void ReleaseDrivers();

		//
		// Swap the back buffers
		void SwapBuffers();

		/*!
			\brief Process this windows internal events
		*/
		virtual bool ProcessEvents();

	public:
		virtual ~Win32GLActiveWindow();
		
		/*!
			\brief Retrieves this windows render context
		*/
		inline Win32RenderContext* GetRenderContext() {
			return mRenderContext;
		}

	private:
		IConfiguration* mConfiguration;
		HDC mDeviceContext;
		Win32RenderContext* mRenderContext;
		HWND mWindowHandle;
	};
}
