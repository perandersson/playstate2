#include "../../Memory.h"
#include "Win32GLActiveWindow.h"
#include "../../rendering/exception/RenderingException.h"
#include "../../rendering/WGLEWMX.h"
using namespace core;

Win32GLActiveWindow::Win32GLActiveWindow()
: mRenderContext(nullptr), mWindowHandle(NULL)
{

}

Win32GLActiveWindow::~Win32GLActiveWindow()
{
	assert_null(mRenderContext);
}

void Win32GLActiveWindow::InitializeDrivers(HWND windowHandle, const Size& size)
{
	mWindowHandle = windowHandle;

	PIXELFORMATDESCRIPTOR pfd = { 0 };
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;

	mDeviceContext = GetDC(mWindowHandle);
	if (!mDeviceContext) {
		THROW_EXCEPTION(RenderingException, "Could not create a device context for the current window");
	}

	int format = ChoosePixelFormat(mDeviceContext, &pfd);
	if (!format) {
		THROW_EXCEPTION(RenderingException, "Could not generate pixel format for the current window");
	}

	if (!SetPixelFormat(mDeviceContext, format, &pfd)) {
		THROW_EXCEPTION(RenderingException, "Could not set pixel format for the current window");
	}

	// Creates a temporary context. It's needed so that we can use
	// OpenGL related extensions. This will be removed as soon as 
	// a valid GL 3.3 context is created.
	HGLRC tempContext = wglCreateContext(mDeviceContext);
	if (!tempContext) {
		THROW_EXCEPTION(RenderingException, "Could not create an opengl rendering context for the current window");
	}

	wglMakeCurrent(mDeviceContext, tempContext);

	//
	// Initialize GLEW temporarily for this render context
	//

	std::shared_ptr<GLEWContext> tempGLEWContext(new GLEWContext); 
	memset(tempGLEWContext.get(), 0, sizeof(GLEWContext));
	std::shared_ptr<WGLEWContext> tempWGLEWContext(new WGLEWContext); 
	memset(tempWGLEWContext.get(), 0, sizeof(WGLEWContext));

	SetGLEWContext(tempGLEWContext.get());
	SetWGLEWContext(tempWGLEWContext.get());

	glewExperimental = GL_TRUE;
	GLenum result = glewInit();
	if (result != GLEW_OK) {
		THROW_EXCEPTION(RenderingException, "Could not initialize GLEW");
	}

	result = wglewInit();
	if (result != GLEW_OK) {
		THROW_EXCEPTION(RenderingException, "Could not initialize WGLEW");
	}

	if (wglewIsSupported("WGL_ARB_create_context") != 1) {
		THROW_EXCEPTION(RenderingException, "You'r graphics card does not support OpenGL 3.3");
	}

	int attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_FLAGS_ARB, 0,
		0
	};

	HGLRC windowsRenderContext = wglCreateContextAttribsARB(mDeviceContext, NULL, attribs);
	if (!windowsRenderContext) {
		THROW_EXCEPTION(RenderingException, "You'r graphics card does not support OpenGL 3.3");
	}
	wglDeleteContext(tempContext);

	//
	// Make sure that the temporary GLEW contexts are unbound
	//

	SetGLEWContext(nullptr);
	SetWGLEWContext(nullptr);

	//
	// Create and bind the OpenGL 3.3 render context
	//

	mRenderContext = new Win32RenderContext(mDeviceContext, windowsRenderContext);
	Win32RenderContext::SetThreadLocal(mRenderContext);
	mRenderContext->Bind();
}

void Win32GLActiveWindow::ReleaseDrivers()
{
	if (mRenderContext != nullptr) {
		delete mRenderContext;
		mRenderContext = nullptr;
	}

	if (mDeviceContext != NULL) {
		ReleaseDC(mWindowHandle, mDeviceContext);
		mDeviceContext = NULL;
		mWindowHandle = NULL;
	}
}

void Win32GLActiveWindow::SwapBuffers()
{
	::SwapBuffers(mDeviceContext);
	GLenum error = glGetError();
	assert(error == GL_NO_ERROR);
}

bool Win32GLActiveWindow::ProcessEvents()
{
	SwapBuffers();
	return true;
}
