#pragma once
#include "../typedefs.h"
#include <gl/glew.h>

namespace core
{
	class VertexBuffer;

	class IndexBuffer
	{
	public:
		IndexBuffer(GLuint bufferID, uint32 numElements);
		~IndexBuffer();

		inline uint32 GetUID() const {
			return mUID;
		}

		inline GLuint GetBufferID() const {
			return mBufferID;
		}

		inline uint32 GetNumElements() const {
			return mNumElements;
		}

		void Render(VertexBuffer* buffer) const;
		void Render(VertexBuffer* buffer, uint32 firstElement) const;
		void Render(VertexBuffer* buffer, uint32 firstElement, uint32 numElements) const;

	private:
		uint32 mUID;
		GLuint mBufferID;
		uint32 mNumElements;
	};
}