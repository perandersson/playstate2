#pragma once
#include "CompareFunc.h"
#include "CompareMode.h"
#include "MinFilter.h"
#include "MagFilter.h"
#include "TextureWrap.h"
#include "TextureFormat.h"
#include "BufferUsage.h"
#include "VertexType.h"
#include "DepthFunc.h"
#include "BlendFunc.h"
#include "FrontFace.h"
#include "CullFace.h"
#include "PolygonMode.h"
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

		static GLenum Convert(DepthFunc::Enum e);
		static GLenum Convert(SrcFactor::Enum e);
		static GLenum Convert(DestFactor::Enum e);
		static GLenum Convert(FrontFace::Enum e);
		static GLenum Convert(CullFace::Enum e);
		static GLenum Convert(PolygonMode::Enum e);
		static GLenum Convert(PrimitiveType::Enum e);

		static GLenum ConvertToTextureFormatEnum(TextureFormat::Enum format);
		static GLenum ConvertToInternalTextureFormatEnum(TextureFormat::Enum format);
	};
}
