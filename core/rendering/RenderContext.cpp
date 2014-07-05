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

IndexBuffer* RenderContext::CreateBuffer(const uint32* indices, uint32 numIndices, BufferUsage::Enum usage)
{
	return Kernel::GetRenderContext()->CreateBuffer(indices, numIndices, usage);
}

VertexBuffer* RenderContext::CreateBuffer(const PositionTextureVertexType* vertices, PrimitiveType::Enum primitiveType, uint32 numVertices, BufferUsage::Enum usage)
{
	return Kernel::GetRenderContext()->CreateBuffer(vertices, primitiveType, numVertices, usage);
}

VertexBuffer* RenderContext::CreateBuffer(const PositionTextureNormalVertexType* vertices, PrimitiveType::Enum primitiveType, uint32 numVertices, BufferUsage::Enum usage)
{
	return Kernel::GetRenderContext()->CreateBuffer(vertices, primitiveType, numVertices, usage);
}

VertexBuffer* RenderContext::CreateBuffer(const void* vertices, uint32 sizeOfOneVertex, const VertexDesc& vertexDesc, PrimitiveType::Enum primitiveType, uint32 numVertices, BufferUsage::Enum usage)
{
	return Kernel::GetRenderContext()->CreateBuffer(vertices, sizeOfOneVertex, vertexDesc, primitiveType, numVertices, usage);
}

RenderTarget2D* RenderContext::CreateRenderTarget2D(const Size& size, TextureFormat::Enum format)
{
	return Kernel::GetRenderContext()->CreateRenderTarget2D(size, format);
}

Texture2D* RenderContext::CreateTexture2D(const Size& size, TextureFormat::Enum format, const byte* bytes)
{
	return Kernel::GetRenderContext()->CreateTexture2D(size, format, bytes);
}

RenderTargetCube* RenderContext::CreateRenderTargetCube(const Size& size, TextureFormat::Enum format)
{
	return Kernel::GetRenderContext()->CreateRenderTargetCube(size, format);
}

TextureCube* RenderContext::CreateTextureCube(const Size& size, TextureFormat::Enum format, const byte* positiveX, const byte* negativeX,
	const byte* positiveY, const byte* negativeY, const byte* positiveZ, const byte* negativeZ)
{
	return Kernel::GetRenderContext()->CreateTextureCube(size, format, positiveX, negativeX, positiveY, negativeY, positiveZ, negativeZ);
}
