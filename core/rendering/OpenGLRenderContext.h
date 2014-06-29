#pragma once
#include "../typedefs.h"
#include "../filesystem/IFile.h"
#include "IRenderContext.h"
#include "MinFilter.h"
#include "MagFilter.h"
#include "TextureWrap.h"
#include "TextureFormat.h"
#include "../math/Point.h"
#include "GraphicsCapabilities.h"
#include "exception/RenderingException.h"
#include "../resource/Resource.h"
#include <gl/glew.h>
#include <future>

namespace core
{
	/*!
		\brief 
	*/
	class OpenGLRenderContext : public IRenderContext
	{
	public:
		OpenGLRenderContext();
		virtual ~OpenGLRenderContext();

		virtual RenderState* GetRenderState();
		virtual RenderState* Activate(Effect* effect);
		virtual RenderState* Activate(Resource<Effect> effect);
		virtual IndexBuffer* CreateStaticBuffer(const uint32* indices, uint32 numIndices);
		virtual VertexBuffer* CreateStaticBuffer(const PositionTextureVertexType* vertices, uint32 numVertices);
		virtual VertexBuffer* CreateStaticBuffer(const PositionTextureNormalVertexType* vertices, uint32 numVertices);
		virtual VertexBuffer* CreateStaticBuffer(const void* vertices, uint32 sizeOfOneVertex, const VertexDesc& vertexDesc, uint32 numVertices);
		virtual RenderTarget2D* CreateRenderTarget2D(const Size& size, TextureFormat::Enum format);
		virtual Texture2D* CreateTexture2D(const Size& size, TextureFormat::Enum format, const byte* bytes);

	private:
		static GLenum GetTextureFormatAsEnum(TextureFormat::Enum format);
		static GLenum GetInternalTextureFormatAsEnum(TextureFormat::Enum format);
		static GLenum GetMinFilterAsEnum(MinFilter::Enum minFilter);
		static GLenum GetMagFilterAsEnum(MagFilter::Enum magFilter);
		static GLenum GetTextureWrapAsEnum(TextureWrap::Enum textureWrap);

		GLuint GenTextureID() const;
		GLuint GenBufferID() const;

	private:
		std::vector<std::shared_ptr<RenderState>> mRenderStatesCreatedByContext;
	};

}
