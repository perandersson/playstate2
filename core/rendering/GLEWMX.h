#pragma once

#define glewGetContext GetGLEWContext

#include <GL/glew.h>

/*!
	\brief Retrieves a pointer to the GLEW context, used by GLEW when render from multiple threads
*/
extern GLEWContext* GetGLEWContext();
extern void SetGLEWContext(GLEWContext* context);
