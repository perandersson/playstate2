#pragma once
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"

namespace core
{
	/*!
	
	*/
	class Sphere
	{
	public:
		Sphere(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, float32 radius, uint32 resolution);
		~Sphere();

		inline const VertexBuffer* GetVertexBuffer() const {
			return mVertexBuffer;
		}

		inline const IndexBuffer* GetIndexBuffer() const {
			return mIndexBuffer;
		}

		inline float32 GetRadius() const {
			return mRadius;
		}

		inline uint32 GetResolution() const {
			return mResolution;
		}

	private:
		VertexBuffer* mVertexBuffer; 
		IndexBuffer* mIndexBuffer; 
		float32 mRadius;
		uint32 mResolution;

	public:
		/*!
			\brief Creates a sphere

			\param radius
					The radius of the sphere
			\param resolution
					The quality of the sphere (how smooth it is)
			\param usage
					What are we planning on doing with the vertex- and index buffers
		*/
		static Sphere* Create(float32 radius, uint32 resolution, BufferUsage::Enum usage);
	};
}
