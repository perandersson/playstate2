#pragma once
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../VertexTypes.h"
#include "../../math/Vector3.h"

namespace core
{
	class Cone
	{
	public:
		Cone(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, const Vector3& apex, const Vector3& direction, float32 height, float32 radius, uint32 resolution);
		~Cone();

		inline const VertexBuffer* GetVertexBuffer() const {
			return mVertexBuffer;
		}

		inline const IndexBuffer* GetIndexBuffer() const {
			return mIndexBuffer;
		}

		inline const Vector3& GetApex() const {
			return mApex;
		}

		inline const Vector3& GetDirection() const {
			return mDirection;
		}

		inline float32 GetHeight() const {
			return mHeight;
		}

		inline float32 GetRadius() const {
			return mRadius;
		}

		inline uint32 GetResolution() const {
			return mResolution;
		}

		/*!
			\brief Updates this cone shape with the supplied properties

			\param apex
					The apex position
			\param direction
					The normalized direction from the apex's point-of-view
			\param height
					The height of the cone shape
			\param radius
					The radius
			\param resolution
					The resolution for this shape (how smooth it is)
			\param usage
					What are we planning on doing with the vertex- and index buffers
		*/
		void Update(const Vector3& apex, const Vector3& direction, float32 height, float32 radius, uint32 resolution);

	private:
		VertexBuffer* mVertexBuffer;
		IndexBuffer* mIndexBuffer;
		Vector3 mApex;
		Vector3 mDirection;
		float32 mHeight;
		float32 mRadius;
		uint32 mResolution;

	public:
		/*!
			\brief Creates a cone shape

			\param apex
					The apex position
			\param direction
					The normalized direction from the apex's point-of-view
			\param height
					The height of the cone shape
			\param radius
					The radius
			\param resolution
					The resolution for this shape (how smooth it is)
			\param usage
					What are we planning on doing with the vertex- and index buffers
		*/
		static Cone* Create(const Vector3& apex, const Vector3& direction, float32 height, float32 radius, uint32 resolution, BufferUsage::Enum usage);

	private:
		/*!
			\brief Generate vertices neccessary for a cone-model based on the supplied parameters
			
			\param apex
					The apex position
			\param direction
					Normalized directional vector from the surface to the apex
			\param height
					The height of the cone shape
			\param radius
					The radius
			\param resolution
					The resolution for this shape (how smooth it is)
			\param _out_Vertices
					The vector that will be filled with the new vertices
		*/
		static void GenerateVertices(const Vector3& apex, const Vector3& direction, float32 height, float32 radius, uint32 resolution, std::vector<PositionVertexType>& _out_Vertices);
	};

}
