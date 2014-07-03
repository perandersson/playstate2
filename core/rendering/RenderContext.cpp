#include "../Memory.h"
#include "RenderContext.h"
#include "../kernel/Kernel.h"
using namespace core;

void RenderContext::Bind()
{
	Kernel::GetRenderContext()->Bind();
}

void RenderContext::Unbind()
{
	Kernel::GetRenderContext()->Unbind();
}

RenderState* RenderContext::GetRenderState()
{
	return Kernel::GetRenderContext()->GetRenderState();
}

RenderState* RenderContext::Activate(Effect* effect)
{
	return Kernel::GetRenderContext()->Activate(effect);
}

RenderState* RenderContext::Activate(Resource<Effect> effect)
{
	return Kernel::GetRenderContext()->Activate(effect);
}

IndexBuffer* RenderContext::CreateStaticBuffer(const uint32* indices, uint32 numIndices)
{
	return Kernel::GetRenderContext()->CreateStaticBuffer(indices, numIndices);
}

VertexBuffer* RenderContext::CreateStaticBuffer(const PositionTextureVertexType* vertices, uint32 numVertices)
{
	return Kernel::GetRenderContext()->CreateStaticBuffer(vertices, numVertices);
}

VertexBuffer* RenderContext::CreateStaticBuffer(const PositionTextureNormalVertexType* vertices, uint32 numVertices)
{
	return Kernel::GetRenderContext()->CreateStaticBuffer(vertices, numVertices);
}

VertexBuffer* RenderContext::CreateStaticBuffer(const void* vertices, uint32 sizeOfOneVertex, const VertexDesc& vertexDesc, uint32 numVertices)
{
	return Kernel::GetRenderContext()->CreateStaticBuffer(vertices, sizeOfOneVertex, vertexDesc, numVertices);
}

RenderTarget2D* RenderContext::CreateRenderTarget2D(const Size& size, TextureFormat::Enum format)
{
	return Kernel::GetRenderContext()->CreateRenderTarget2D(size, format);
}

Texture2D* RenderContext::CreateTexture2D(const Size& size, TextureFormat::Enum format, const byte* bytes)
{
	return Kernel::GetRenderContext()->CreateTexture2D(size, format, bytes);
}

TextureCube* RenderContext::CreateTextureCube(const Size& size, TextureFormat::Enum format, const byte* positiveX, const byte* negativeX,
	const byte* positiveY, const byte* negativeY, const byte* positiveZ, const byte* negativeZ)
{
	return Kernel::GetRenderContext()->CreateTextureCube(size, format, positiveX, negativeX, positiveY, negativeY, positiveZ, negativeZ);
}
