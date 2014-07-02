#include "../Memory.h"
#include "VertexType.h"
using namespace core;

namespace {
	bool IsIntegerType(const VertexElementDesc& elemDesc)
	{
		const VertexElementType::Enum type = elemDesc.type;
		return type == VertexElementType::INT || type == VertexElementType::UNSIGNED_INT || type == VertexElementType::BYTE
			|| type == VertexElementType::UNSIGNED_BYTE || type == VertexElementType::SHORT || type == VertexElementType::UNSIGNED_SHORT;
	}
}

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
