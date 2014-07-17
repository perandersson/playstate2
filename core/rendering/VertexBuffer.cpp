#include "../Memory.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RenderContext.h"
#include "RenderState.h"
#include "exception/RenderingException.h"
#include <atomic>
using namespace core;

namespace {
	static std::atomic<uint32> ids(0);
	uint32 GenBufferUID() {
		return ++ids;
	}
}

VertexBuffer::VertexBuffer(GLuint bufferID, const VertexDesc& vertexDesc, PrimitiveType::Enum primitiveType, uint32 numVertices, uint32 sizeOfOneVertex, BufferUsage::Enum bufferUsage)
: mUID(GenBufferUID()), mBufferID(bufferID), mVertexDesc(vertexDesc), mPrimitiveType(primitiveType), mNumVertices(numVertices), mSizeOfOneVertex(sizeOfOneVertex), mBufferUsage(bufferUsage)
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

	glDrawArrays(GetPrimitiveTypeEnum(mPrimitiveType), startIndex, count);
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

	const uint32 count = numElements / PrimitiveType::GetElementCount(mPrimitiveType);
	const GLenum mode = GetPrimitiveTypeEnum(mPrimitiveType);
	glDrawElements(mode, count, GL_UNSIGNED_INT, (void*)(firstElement * sizeof(uint32)));
}

void VertexBuffer::Update(const void* vertices, uint32 numVertices)
{
	assert_not_null(vertices);
	assert(numVertices > 0 && "Why update 0 vertices?");
	assert(mBufferUsage == BufferUsage::DYNAMIC && "Updating a non-dynamic buffer has major performance penalties. Do not do this!");

	//
	// Get the current rendering state
	//

	RenderState* renderState = RenderContext::GetRenderState();
	renderState->BindVertexBuffer(this);

	//
	// Update data
	//

	glBufferData(GL_ARRAY_BUFFER, mNumVertices * mSizeOfOneVertex, vertices, GL_DYNAMIC_DRAW);
	glFlush();

	GLenum status = glGetError();
	if (status != GL_NO_ERROR) {
		THROW_EXCEPTION(RenderingException, "Could not update vertex buffer. Reason: %d", status);
	}

	//
	// Mark the current vertex buffer as dirty
	//
	
	renderState->UnbindVertexBuffer();
}

GLenum VertexBuffer::GetPrimitiveTypeEnum(PrimitiveType::Enum primitiveType)
{
	static const GLenum enums[PrimitiveType::SIZE] = {
		GL_POINTS,
		GL_TRIANGLES,
		GL_TRIANGLE_STRIP,
		GL_LINE_LOOP,
	};
	return enums[primitiveType];
}
