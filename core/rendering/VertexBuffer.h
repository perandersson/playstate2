#pragma once
#include "../typedefs.h"
#include "VertexType.h"
#include "GLEWMX.h"

namespace core
{
	class IndexBuffer;

	class VertexBuffer
	{
	public:
		VertexBuffer(GLuint bufferID, const VertexDesc& vertexDesc, uint32 numVertices, uint32 sizeOfOneVertex);
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

		void Render() const;
		void Render(uint32 startIndex) const;
		void Render(uint32 startIndex, uint32 count) const;

		void Render(const IndexBuffer* indexBuffer) const;
		void Render(const IndexBuffer* indexBuffer, uint32 startIndex) const;
		void Render(const IndexBuffer* indexBuffer, uint32 startIndex, uint32 count) const;

		static GLenum GetPrimitiveTypeEnum(PrimitiveType::Enum primitiveType);

	private:
		uint32 mUID;
		GLuint mBufferID;
		const VertexDesc& mVertexDesc;
		uint32 mNumVertices;
		uint32 mSizeOfOneVertex;
	};
}
