#pragma once
#include "IRenderContext.h"
#include "../kernel/Kernel.h"
#include <future>

namespace core
{
	/*!
		\brief Helper class for the current thread's render context
	*/
	class RenderContext
	{
	public:
		static void Bind();
		static void Unbind();
		static RenderState* GetRenderState();
		static RenderState* Activate(Effect* effect);
		static RenderState* Activate(Resource<Effect> effect);
		static IndexBuffer* CreateBuffer(const uint32* indices, uint32 numIndices, BufferUsage::Enum usage);
		static VertexBuffer* CreateBuffer(const PositionTextureVertexType* vertices, PrimitiveType::Enum primitiveType, uint32 numVertices, BufferUsage::Enum usage);
		static VertexBuffer* CreateBuffer(const PositionTextureNormalVertexType* vertices, PrimitiveType::Enum primitiveType, uint32 numVertices, BufferUsage::Enum usage);
		static VertexBuffer* CreateBuffer(const void* vertices, uint32 sizeOfOneVertex, const VertexDesc& vertexDesc, PrimitiveType::Enum primitiveType, uint32 numVertices, BufferUsage::Enum usage);
		static RenderTarget2D* CreateRenderTarget2D(const Size& size, TextureFormat::Enum format);
		static Texture2D* CreateTexture2D(const Size& size, TextureFormat::Enum format, const byte* bytes);
		static RenderTargetCube* CreateRenderTargetCube(const Size& size, TextureFormat::Enum format);
		static TextureCube* CreateTextureCube(const Size& size, TextureFormat::Enum format, const byte* positiveX, const byte* negativeX,
			const byte* positiveY, const byte* negativeY, const byte* positiveZ, const byte* negativeZ);
		
		/*!
			\brief Call the supplied function in a separate thread.

			Binds this context to the new thread before calling the supplied function

			\tparam Result
					The result the supplied function returns
			\tparam Func
					The function type
			\param func
						The function we want to call from the new thread
		*/
		template<typename Result, typename Func>
		static std::future<Result> Async(Func func) {
			return Kernel::GetThreadPool()->AddTask([func] {
				IRenderContext* ctx = Kernel::GetRenderContext();
				ctx->Bind();
				Result result = func();
				ctx->Unbind();
				return result;
			});
		}
	};
}
