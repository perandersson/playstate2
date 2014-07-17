#include "../Memory.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "RenderContext.h"
#include "RenderState.h"
#include "exception/RenderingException.h"
#include <atomic>
using namespace core;

namespace {
	static std::atomic<uint32> ids(0);
	uint32 GenIndexBufferUID() {
		return ++ids;
	}
}

IndexBuffer::IndexBuffer(GLuint bufferID, uint32 numElements, BufferUsage::Enum bufferUsage)
: mUID(GenIndexBufferUID()), mBufferID(bufferID), mNumElements(numElements), mBufferUsage(bufferUsage)
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

void IndexBuffer::Update(const uint32* indices, uint32 numIndices)
{
	assert_not_null(indices);
	assert(numIndices > 0 && "Why update 0 numIndices?");
	assert(mBufferUsage == BufferUsage::DYNAMIC && "Updating a non-dynamic buffer has major performance penalties. Do not do this!");

	//
	// Get the current rendering state
	//

	RenderState* renderState = RenderContext::GetRenderState();
	renderState->BindIndexBuffer(this);

	//
	// Update data
	//

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(uint32), indices, GL_DYNAMIC_DRAW);
	glFlush();

	GLenum status = glGetError();
	if (status != GL_NO_ERROR) {
		THROW_EXCEPTION(RenderingException, "Could not update index buffer. Reason: %d", status);
	}

	//
	// Mark the current vertex buffer as dirty
	//
	
	renderState->UnbindIndexBuffer();
}
