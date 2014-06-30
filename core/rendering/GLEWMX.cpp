#include "GLEWMX.h"
#include "../ThreadLocal.h"

THREAD_LOCAL GLEWContext* gGLEWContext = nullptr;

GLEWContext* GetGLEWContext()
{
	return gGLEWContext;
}


void SetGLEWContext(GLEWContext* context)
{
	gGLEWContext = context;
}
