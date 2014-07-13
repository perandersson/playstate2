#pragma once
#include "../typedefs.h"
#include "VertexType.h"
#include "BufferUsage.h"
#include "GLEWMX.h"

namespace core
{
	class IndexBuffer;

	class VertexBuffer
	{
	public:
		VertexBuffer(GLuint bufferID, const VertexDesc& vertexDesc, PrimitiveType::Enum primitiveType, uint32 numVertices, uint32 sizeOfOneVertex, BufferUsage::Enum bufferUsage);
		~VertexBuffer();

		inline uint32 GetUID() const {
			return mUID;
		}

		inline GLuint GetBufferID() const {
			return mBufferID;
		}

		inline const VertexDesc& GetVertexDesc() const {
			return mVertexDesc;
		}

		inline uint32 GetNumVertices() const {
			return mNumVertices;
		}

		inline BufferUsage::Enum GetBufferUsage() const {
			return mBufferUsage;
		}

		void Render() const;
		void Render(uint32 startIndex) const;
		void Render(uint32 startIndex, uint32 count) const;

		void Render(const IndexBuffer* indexBuffer) const;
		void Render(const IndexBuffer* indexBuffer, uint32 startIndex) const;
		void Render(const IndexBuffer* indexBuffer, uint32 startIndex, uint32 count) const;

		/*!
			\brief Update this vertex buffer with new data

			It's not possible to change how the actual vertex data is structured this way

			\param vertices
			\param numVertices
		*/
		void Update(const void* vertices, uint32 numVertices);

		static GLenum GetPrimitiveTypeEnum(PrimitiveType::Enum primitiveType);

	private:
		uint32 mUID;
		GLuint mBufferID;
		const VertexDesc& mVertexDesc;
		PrimitiveType::Enum mPrimitiveType;
		uint32 mNumVertices;
		uint32 mSizeOfOneVertex;
		BufferUsage::Enum mBufferUsage;
	};
}
