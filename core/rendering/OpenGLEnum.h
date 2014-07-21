#pragma once
#include "CompareFunc.h"
#include "CompareMode.h"
#include "MinFilter.h"
#include "MagFilter.h"
#include "TextureWrap.h"
#include "TextureFormat.h"
#include "BufferUsage.h"
#include "VertexType.h"
#include "GLEWMX.h"

namespace core
{
	class OpenGLEnum
	{
	public:
		static GLenum Convert(CompareFunc::Enum e);
		static GLenum Convert(CompareMode::Enum e);
		static GLenum Convert(MagFilter::Enum e);
		static GLenum Convert(MinFilter::Enum e);
		static GLenum Convert(TextureWrap::Enum e);
		static GLenum Convert(BufferUsage::Enum e);
		static GLenum Convert(VertexElementType::Enum e);
	};
}
