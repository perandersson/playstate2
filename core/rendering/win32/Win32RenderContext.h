#pragma once
#include "../OpenGLRenderContext.h"
#include "../../ThreadLocal.h"

#include <Windows.h>
#include "WGLEWMX.h"

namespace core
{
	class Win32RenderContext : public OpenGLRenderContext, public ThreadLocal<IRenderContext*>
	{
	public:
		Win32RenderContext(HDC deviceContext, HGLRC renderContext);
		virtual ~Win32RenderContext();

		inline HGLRC GetRenderContext() const {
			return mRenderContext;
		}
		
		/*!
			\brief Creates- and connects a new render context to this render context

			Creates- and connects a new render context to this render context so that the contexts internal resources are shared. 
			This is useful for when trying to perform graphics related functionality from multiple threads
			while later be able to use them in the main thread.

			\return A new render context instance
		*/
		virtual OpenGLRenderContext* CreateRenderContext();

		virtual void Bind();
		virtual void Unbind();

	private:
		HDC mDeviceContext;
		HGLRC mRenderContext;

		// Vector containing all the child contexts created by this render context. Useful so that 
		// we can safely cleanup all the memory we've been using
		std::vector<std::shared_ptr<OpenGLRenderContext>> mRenderContextsCreatedByContext;

		WGLEWContext* mWGLEWContext;
	};
}
