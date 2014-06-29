#pragma once
#include "../resource/IResourceLoader.h"
#include "IRenderContext.h"

namespace core
{
	class RenderContextResourceLoader : public IResourceLoader
	{
	public:
		RenderContextResourceLoader();
		virtual ~RenderContextResourceLoader();

	protected:
		IRenderContext* GetRenderContext();
	};
}
