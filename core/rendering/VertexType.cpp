#include "../Memory.h"
#include "VertexType.h"
using namespace core;

GLenum VertexElementType::Parse(Enum e)
{
	static const GLenum enums[VertexElementType::SIZE] = {
		GL_BYTE,
		GL_UNSIGNED_BYTE,
		GL_SHORT,
		GL_UNSIGNED_SHORT,
		GL_INT,
		GL_UNSIGNED_INT,
		GL_FLOAT,
		GL_DOUBLE
	};

	return enums[(uint32)e];
}
