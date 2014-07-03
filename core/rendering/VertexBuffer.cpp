#include "../Memory.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <atomic>
using namespace core;

namespace {
	static std::atomic<uint32> ids(0);
	uint32 GenBufferUID() {
		return ++ids;
	}
}

VertexBuffer::VertexBuffer(GLuint bufferID, const VertexDesc& vertexDesc, uint32 numVertices, uint32 sizeOfOneVertex)
: mUID(GenBufferUID()), mBufferID(bufferID), mVertexDesc(vertexDesc), mNumVertices(numVertices), mSizeOfOneVertex(sizeOfOneVertex)
{
	assert(mNumVertices > 0 && "Why create a vertex buffer without any vertices in it?");
	assert(mSizeOfOneVertex > 0 && "The size of one vertex cannot be 0");
}

VertexBuffer::~VertexBuffer()
{
	if (mBufferID != 0) {
		glDeleteBuffers(1, &mBufferID);
		mBufferID = 0;
	}
}

void VertexBuffer::Render() const
{
	Render((uint32)0);
}

void VertexBuffer::Render(uint32 startIndex) const
{
	Render((uint32)startIndex, (uint32)mNumVertices);
}

void VertexBuffer::Render(uint32 startIndex, uint32 count) const
{
	// Prevent you from drawing more vertices than exist in the buffer
	if (count > mNumVertices - startIndex) {
		count = mNumVertices - startIndex;
	}

	glDrawArrays(GetPrimitiveTypeEnum(mVertexDesc.primitiveType), startIndex, count);
}

void VertexBuffer::Render(const IndexBuffer* buffer) const
{
	assert(buffer != NULL && "No index buffer was supplied");
	Render(buffer, 0);
}

void VertexBuffer::Render(const IndexBuffer* buffer, uint32 firstElement) const
{
	assert(buffer != NULL && "No index buffer was supplied");
	Render(buffer, firstElement, buffer->GetNumElements());
}

void VertexBuffer::Render(const IndexBuffer* buffer, uint32 firstElement, uint32 numElements) const
{
	assert(buffer != NULL && "No index buffer was supplied");

	// So far only the GL_UNSIGNED_INT indice type is usable
	glDrawElements(GetPrimitiveTypeEnum(mVertexDesc.primitiveType), numElements, GL_UNSIGNED_INT, (void*)(firstElement * sizeof(uint32)));
}

GLenum VertexBuffer::GetPrimitiveTypeEnum(PrimitiveType::Enum primitiveType)
{
	static const GLenum enums[PrimitiveType::SIZE] = {
		GL_POINTS,
		GL_TRIANGLES,
		GL_LINE_LOOP
	};
	return enums[primitiveType];
}
