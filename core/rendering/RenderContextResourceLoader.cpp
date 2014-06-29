#include "../Memory.h"
#include "RenderContextResourceLoader.h"
#include "../kernel/Kernel.h"
using namespace core;

RenderContextResourceLoader::RenderContextResourceLoader()
{
}

RenderContextResourceLoader::~RenderContextResourceLoader()
{
}

IRenderContext* RenderContextResourceLoader::GetRenderContext()
{
	return Kernel::GetRenderContext();
}
