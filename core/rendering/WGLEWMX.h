#pragma once

#define wglewGetContext GetWGLEWContext

#include <GL/glew.h>
#include <GL/wglew.h>

/*!
	\brief Retrieves a pointer to the GLEW context, used by GLEW when render from multiple threads
*/
extern WGLEWContext * GetWGLEWContext();
extern void SetWGLEWContext(WGLEWContext* context);
