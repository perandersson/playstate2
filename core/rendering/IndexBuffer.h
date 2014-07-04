#pragma once
#include "../typedefs.h"
#include "BufferUsage.h"
#include "GLEWMX.h"

namespace core
{
	class VertexBuffer;

	class IndexBuffer
	{
	public:
		IndexBuffer(GLuint bufferID, uint32 numElements, BufferUsage::Enum bufferUsage);
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
		
		/*!
			\brief Update this index buffer with new data

			\param indices
			\param numIndices
		*/
		void Update(const uint32* indices, uint32 numIndices);

	private:
		uint32 mUID;
		GLuint mBufferID;
		uint32 mNumElements;
		BufferUsage::Enum mBufferUsage;
	};
}