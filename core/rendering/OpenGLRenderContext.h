#pragma once
#include "../typedefs.h"
#include "../filesystem/IFile.h"
#include "IRenderContext.h"
#include "MinFilter.h"
#include "MagFilter.h"
#include "TextureWrap.h"
#include "TextureFormat.h"
#include "../math/Point.h"
#include "exception/RenderingException.h"
#include "../resource/Resource.h"
#include "GLEWMX.h"
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
		virtual IndexBuffer* CreateBuffer(const uint32* indices, uint32 numIndices, BufferUsage::Enum usage);
		virtual VertexBuffer* CreateBuffer(const PositionVertexType* vertices, PrimitiveType::Enum primitiveType, uint32 numVertices, BufferUsage::Enum usage);
		virtual VertexBuffer* CreateBuffer(const PositionTextureVertexType* vertices, PrimitiveType::Enum primitiveType, uint32 numVertices, BufferUsage::Enum usage);
		virtual VertexBuffer* CreateBuffer(const PositionTextureNormalVertexType* vertices, PrimitiveType::Enum primitiveType, uint32 numVertices, BufferUsage::Enum usage);
		virtual VertexBuffer* CreateBuffer(const void* vertices, uint32 sizeOfOneVertex, const VertexDesc& vertexDesc, PrimitiveType::Enum primitiveType, uint32 numVertices, BufferUsage::Enum usage);
		virtual RenderTarget2D* CreateRenderTarget2D(const Size& size, TextureFormat::Enum format);
		virtual Texture2D* CreateTexture2D(const Size& size, TextureFormat::Enum format, const byte* bytes);
		virtual void ResizeTexture2D(Texture2D* texture, const Size& newSize);
		virtual RenderTargetCube* CreateRenderTargetCube(const Size& size, TextureFormat::Enum format);
		virtual TextureCube* CreateTextureCube(const Size& size, TextureFormat::Enum format, const byte* positiveX, const byte* negativeX, 
			const byte* positiveY, const byte* negativeY, const byte* positiveZ, const byte* negativeZ);

	public:
		static GLenum GetTextureFormatAsEnum(TextureFormat::Enum format);
		static GLenum GetInternalTextureFormatAsEnum(TextureFormat::Enum format);

		GLuint GenTextureID() const;
		GLuint GenBufferID() const;

	private:
		// Vector containing all the states created by this render context. Useful so that 
		// we can safely cleanup all the memory we've been using
		std::vector<std::shared_ptr<RenderState>> mRenderStatesCreatedByContext;

	protected:
		GLEWContext* mGLEWContext;
	};

}
