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

GLenum OpenGLEnum::Convert(DepthFunc::Enum depthFunc)
{
	static const GLenum enums[DepthFunc::SIZE] = {
		GL_NEVER,
		GL_LESS,
		GL_EQUAL,
		GL_LEQUAL,
		GL_GREATER,
		GL_NOTEQUAL,
		GL_GEQUAL,
		GL_ALWAYS
	};

	return enums[(uint32)depthFunc];
}

GLenum OpenGLEnum::Convert(SrcFactor::Enum sfactor)
{
	static const GLenum enums[SrcFactor::SIZE] = {
		GL_ZERO,
		GL_ONE,
		GL_SRC_COLOR,
		GL_ONE_MINUS_SRC_COLOR,
		GL_SRC_ALPHA,
		GL_ONE_MINUS_SRC_ALPHA,
		GL_DST_ALPHA,
		GL_ONE_MINUS_DST_ALPHA,
		GL_DST_COLOR,
		GL_ONE_MINUS_DST_COLOR,
		GL_CONSTANT_COLOR,
		GL_ONE_MINUS_CONSTANT_COLOR,
		GL_CONSTANT_ALPHA,
		GL_ONE_MINUS_CONSTANT_ALPHA
	};

	return enums[(uint32)sfactor];
}

GLenum OpenGLEnum::Convert(DestFactor::Enum dfactor)
{
	static const GLenum enums[DestFactor::SIZE] = {
		GL_ZERO,
		GL_ONE,
		GL_SRC_COLOR,
		GL_ONE_MINUS_SRC_COLOR,
		GL_SRC_ALPHA,
		GL_ONE_MINUS_SRC_ALPHA,
		GL_DST_ALPHA,
		GL_ONE_MINUS_DST_ALPHA,
		GL_DST_COLOR,
		GL_ONE_MINUS_DST_COLOR,
		GL_CONSTANT_COLOR,
		GL_ONE_MINUS_CONSTANT_COLOR,
		GL_CONSTANT_ALPHA,
		GL_ONE_MINUS_CONSTANT_ALPHA
	};

	return enums[(uint32)dfactor];
}

GLenum OpenGLEnum::Convert(FrontFace::Enum frontFace)
{
	static const GLenum enums[FrontFace::SIZE] = {
		GL_CW,
		GL_CCW
	};

	return enums[(uint32)frontFace];
}

GLenum OpenGLEnum::Convert(CullFace::Enum cullFace)
{
	static const GLenum enums[CullFace::SIZE] = {
		0,
		GL_FRONT,
		GL_BACK,
		GL_FRONT_AND_BACK,
	};

	return enums[(uint32)cullFace];
}

GLenum OpenGLEnum::Convert(PolygonMode::Enum mode)
{
	static const GLenum enums[PolygonMode::SIZE] = {
		GL_POINT,
		GL_LINE,
		GL_FILL
	};

	return enums[(uint32)mode];
}
