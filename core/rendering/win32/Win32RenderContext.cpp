#include "../../Memory.h"
#include "Win32RenderContext.h"
#include "../RenderState.h"
#include "../exception/RenderingException.h"
using namespace core;

template<> IRenderContext* ThreadLocal<IRenderContext*>::gThreadLocal = nullptr;

Win32RenderContext::Win32RenderContext(HDC deviceContext, HGLRC renderContext)
: OpenGLRenderContext(), mDeviceContext(deviceContext), mRenderContext(renderContext), mWGLEWContext(nullptr)
{
	mWGLEWContext = new WGLEWContext;
	memset(mWGLEWContext, 0, sizeof(WGLEWContext));
}

Win32RenderContext::~Win32RenderContext()
{
	mRenderContextsCreatedByContext.clear();

	if (mRenderContext != nullptr) {
		wglDeleteContext(mRenderContext);
		mRenderContext = nullptr;
	}

	if (mWGLEWContext != nullptr) {
		delete mWGLEWContext;
		mWGLEWContext = nullptr;
	}
}

void Win32RenderContext::Bind()
{
	wglMakeCurrent(mDeviceContext, mRenderContext);

	// If GLEW is not initialized for this thread then register it
	if (GetGLEWContext() == nullptr) {
		SetGLEWContext(mGLEWContext);
		SetWGLEWContext(mWGLEWContext);

		glewExperimental = GL_TRUE;
		GLenum result = glewInit();
		if (result != GLEW_OK) {
			THROW_EXCEPTION(RenderingException, "Could not initialize GLEW");
		}

		result = wglewInit();
		if (result != GLEW_OK) {
			THROW_EXCEPTION(RenderingException, "Could not initialize WGLEW");
		}

		GLenum err = glGetError();
		// Ignore. http://gamedev.stackexchange.com/questions/29990/opengl-glgeterror-returns-invalid-enum-after-call-to-glewinit
	}

	// For a render state to be initialized in this thread (if it doesnt exist)
	GetRenderState();
}

void Win32RenderContext::Unbind()
{
	GetRenderState()->UnbindFrameBufferObject();
	glFlush();
	wglMakeCurrent(NULL, NULL);
}

OpenGLRenderContext* Win32RenderContext::CreateRenderContext()
{
	int attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
#if defined(_DEBUG) || defined(RENDERING_TROUBLESHOOTING)
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
#else
		WGL_CONTEXT_FLAGS_ARB, 0,
#endif
		0
	};

	HGLRC windowsRenderContext = wglCreateContextAttribsARB(mDeviceContext, mRenderContext, attribs);
	if (!windowsRenderContext) {
		THROW_EXCEPTION(RenderingException, "You'r graphics card does not support OpenGL 3.3");
	}
	std::shared_ptr<OpenGLRenderContext> renderContext(new Win32RenderContext(mDeviceContext, windowsRenderContext));
	mRenderContextsCreatedByContext.push_back(renderContext);
	return renderContext.get();
}
