#include "WGLEWMX.h"
#include "../ThreadLocal.h"

THREAD_LOCAL WGLEWContext* gWGLEWContext = nullptr;

WGLEWContext* GetWGLEWContext()
{
	return gWGLEWContext;
}


void SetWGLEWContext(WGLEWContext* context)
{
	gWGLEWContext = context;
}
