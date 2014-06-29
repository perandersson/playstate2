#include "../../Memory.h"
#include "Win32RenderContext.h"
#include "../exception/RenderingException.h"
using namespace core;

template<> IRenderContext* ThreadLocal<IRenderContext*>::gThreadLocal = nullptr;

Win32RenderContext::Win32RenderContext(HDC deviceContext, HGLRC renderContext)
: mDeviceContext(deviceContext), mRenderContext(renderContext)
{
}

Win32RenderContext::~Win32RenderContext()
{
	mRenderContexts.clear();

	if (mRenderContext != nullptr) {
		wglDeleteContext(mRenderContext);
		mRenderContext = nullptr;
	}
}

void Win32RenderContext::Bind()
{
	wglMakeCurrent(mDeviceContext, mRenderContext);
	//SetThreadLocal(this);

	// For a render state to be initialized in this thread (if it doesnt exist)
	GetRenderState();
}

void Win32RenderContext::Unbind()
{
	glFlush();
	//SetThreadLocal(nullptr);
	wglMakeCurrent(NULL, NULL);
}

IRenderContext* Win32RenderContext::CreateRenderContext()
{
	int attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_FLAGS_ARB, 0,
		0
	};

	HGLRC windowsRenderContext = wglCreateContextAttribsARB(mDeviceContext, mRenderContext, attribs);
	if (!windowsRenderContext) {
		THROW_EXCEPTION(RenderingException, "You'r graphics card does not support OpenGL 3.3");
	}
	std::shared_ptr<IRenderContext> renderContext(new Win32RenderContext(mDeviceContext, windowsRenderContext));
	mRenderContexts.push_back(renderContext);
	return renderContext.get();
}
