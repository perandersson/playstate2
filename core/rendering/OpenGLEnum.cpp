#include "../Memory.h"
#include "OpenGLEnum.h"
using namespace core;

GLenum OpenGLEnum::Convert(CompareFunc::Enum e)
{
	static const GLenum enums[CompareFunc::SIZE] = {
		GL_NEVER,
		GL_LESS,
		GL_EQUAL,
		GL_LEQUAL,
		GL_GREATER,
		GL_NOTEQUAL,
		GL_GEQUAL,
		GL_ALWAYS
	};

	return enums[(uint32)e];
}

GLenum OpenGLEnum::Convert(CompareMode::Enum e)
{
	static const GLenum enums[CompareMode::SIZE] = {
		GL_COMPARE_R_TO_TEXTURE,
		GL_NONE
	};

	return enums[(uint32)e];
}

GLenum OpenGLEnum::Convert(MagFilter::Enum e)
{
	static const GLenum enums[MagFilter::SIZE] = {
		GL_NEAREST,
		GL_LINEAR
	};

	return enums[(uint32)e];
}

GLenum OpenGLEnum::Convert(MinFilter::Enum e)
{
	static const GLenum enums[MinFilter::SIZE] = {
		GL_NEAREST,
		GL_LINEAR,
		GL_NEAREST_MIPMAP_NEAREST,
		GL_LINEAR_MIPMAP_NEAREST,
		GL_NEAREST_MIPMAP_LINEAR,
		GL_LINEAR_MIPMAP_LINEAR
	};

	return enums[(uint32)e];
}

GLenum OpenGLEnum::Convert(TextureWrap::Enum e)
{
	static const GLenum enums[TextureWrap::SIZE] = {
		GL_CLAMP_TO_EDGE,
		GL_CLAMP_TO_BORDER,
		GL_MIRRORED_REPEAT,
		GL_REPEAT
	};

	return enums[(uint32)e];
}

GLenum OpenGLEnum::Convert(BufferUsage::Enum e)
{
	static GLenum values[BufferUsage::SIZE] = {
		GL_STATIC_DRAW,
		GL_DYNAMIC_DRAW
	};

	return values[(uint32)e];
}

GLenum OpenGLEnum::Convert(VertexElementType::Enum e)
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
