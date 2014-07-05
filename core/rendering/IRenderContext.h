#pragma once
#include "VertexTypes.h"
#include "PrimitiveType.h"
#include "TextureFormat.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "RenderTarget2D.h"
#include "RenderTargetCube.h"
#include "BufferUsage.h"
#include "../resource/Resource.h"

namespace core
{
	class RenderTarget2D;
	class RenderState;
	class Effect;

	/*!
		\brief
	*/
	class IRenderContext
	{
	public:
		virtual ~IRenderContext() {}

		/*!
			\brief Bind this render context to the current thread
		*/
		virtual void Bind() = 0;

		/*!
			\brief Unbind this context from the current thread
		*/
		virtual void Unbind() = 0;
		
		/*!
			\brief Retrieves the rendering state for the current thread

			\return Retrieves the current threads render state
		*/
		virtual RenderState* GetRenderState() = 0;

		/*!
			\brief Activate the supplied effect and return the current rendering state

			\param effect
			\return
		*/
		virtual RenderState* Activate(Effect* effect) = 0;
		
		/*!
			\brief Activate the supplied effect and return the current rendering state

			\param effect
			\return
		*/
		virtual RenderState* Activate(Resource<Effect> effect) = 0;

		/*!
			\brief Creates an index buffer

			\param indices
			\param numIndices
			\param usage
		*/
		virtual IndexBuffer* CreateBuffer(const uint32* indices, uint32 numIndices, BufferUsage::Enum usage) = 0;

		virtual VertexBuffer* CreateBuffer(const PositionTextureVertexType* vertices, PrimitiveType::Enum primitiveType, uint32 numVertices, BufferUsage::Enum usage) = 0;
		virtual VertexBuffer* CreateBuffer(const PositionTextureNormalVertexType* vertices, PrimitiveType::Enum primitiveType, uint32 numVertices, BufferUsage::Enum usage) = 0;
		virtual VertexBuffer* CreateBuffer(const void* vertices, uint32 sizeOfOneVertex, const VertexDesc& vertexDesc, PrimitiveType::Enum primitiveType, uint32 numVertices, BufferUsage::Enum usage) = 0;

		/*!
			\brief Creates a two-dimensional render target
			
			\param size
						The size of the render target
			\param format
						The texture format of the render target
			\return The render target if the supplied size and format is valid.
			\throws RenderingException if the supplied size or format is invalid
		*/
		virtual RenderTarget2D* CreateRenderTarget2D(const Size& size, TextureFormat::Enum format) = 0;

		/*!
			\brief Creates a two-dimensional texture
			
			\param size
						The size of the texture
			\param format
						The texture's format of
			\param bytes
						The bytes of the texture
			\return The texture if the supplied size and format is valid.
			\throws RenderingException if the supplied size or format is invalid
		*/
		virtual Texture2D* CreateTexture2D(const Size& size, TextureFormat::Enum format, const byte* bytes) = 0;
		
		/*!
			\brief Creates a render target with six sides
			
			\param size
						The size of the render target
			\param format
						The texture format of the render target
			\return The render target if the supplied size and format is valid.
			\throws RenderingException if the supplied size or format is invalid
		*/
		virtual RenderTargetCube* CreateRenderTargetCube(const Size& size, TextureFormat::Enum format) = 0;

		/*!
			\brief Creates a texture with six sides.
			
			\param size
						The size of a side of the cube texture
			\param format
						The texture's format of
			\return The texture if the supplied size and format is valid.
			\throws RenderingException if the supplied size or format is invalid
		*/
		virtual TextureCube* CreateTextureCube(const Size& size, TextureFormat::Enum format, const byte* positiveX, const byte* negativeX,
			const byte* positiveY, const byte* negativeY, const byte* positiveZ, const byte* negativeZ) = 0;
	};
}
