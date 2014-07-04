#include "BufferUsage.h"
using namespace core;

GLenum BufferUsage::Parse(Enum e)
{
	static GLenum values[SIZE] = {
		GL_STATIC_DRAW,
		GL_DYNAMIC_DRAW
	};

	return values[(uint32)e];
}
