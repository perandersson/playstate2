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
		static IndexBuffer* CreateStaticBuffer(const uint32* indices, uint32 numIndices);
		static VertexBuffer* CreateStaticBuffer(const PositionTextureVertexType* vertices, uint32 numVertices);
		static VertexBuffer* CreateStaticBuffer(const PositionTextureNormalVertexType* vertices, uint32 numVertices);
		static VertexBuffer* CreateStaticBuffer(const void* vertices, uint32 sizeOfOneVertex, const VertexDesc& vertexDesc, uint32 numVertices);
		static RenderTarget2D* CreateRenderTarget2D(const Size& size, TextureFormat::Enum format);
		static Texture2D* CreateTexture2D(const Size& size, TextureFormat::Enum format, const byte* bytes);
		
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
