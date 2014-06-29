#include "../Memory.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include <atomic>
using namespace core;

namespace {
	uint32 GenIndexBufferUID() {
		static std::atomic<uint32> ids(0);
		return ++ids;
	}
}

IndexBuffer::IndexBuffer(GLuint bufferID, uint32 numElements)
: mUID(GenIndexBufferUID()), mBufferID(bufferID), mNumElements(numElements)
{
}

IndexBuffer::~IndexBuffer()
{
	if (mBufferID != 0) {
		glDeleteBuffers(1, &mBufferID);
		mBufferID = 0;
	}
}

void IndexBuffer::Render(VertexBuffer* buffer) const
{
	Render(buffer, 0);
}

void IndexBuffer::Render(VertexBuffer* buffer, uint32 firstElement) const
{
	Render(buffer, firstElement, mNumElements);
}

void IndexBuffer::Render(VertexBuffer* buffer, uint32 firstElement, uint32 numElements) const
{
	assert(buffer != NULL && "You are not allowed to render an index buffer without a vertex buffer");
	buffer->Render(this, firstElement, numElements);
}
