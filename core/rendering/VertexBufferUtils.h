#pragma once
#include "VertexBuffer.h"
#include "VertexTypes.h"

namespace core
{
	class VertexBufferUtils
	{
	public:
		/*!
			\brief Create a VertexBuffer containing the vertices needed to draw a sphere

			\param R
					The radius
			\param H
			\param K
			\param Z
			\param usage
			\return
		*/
		static VertexBuffer* CreateSphere(float32 R, uint32 resolution, float32 H, float32 K, float32 Z, BufferUsage::Enum usage);

		/*!
			\brief Create a VertexBuffer containing the vertices needed to draw a cone

			\param d
			\param a
				Position of apex
			\param H
				The height of the cone
			\param Rd
			\param n
					Number of slices
			\return
		*/
		static VertexBuffer* CreateCone(const Vector3& d, const Vector3& a, float32 H, float32 Rd, uint32 n);
		
		/*!
			\brief Create a VertexBuffer containing the vertices needed to draw a cone

			\param position
					Position of the spotlight
			\param direction
					The (normalized) direction where the light is looking at
			\param cutoff
					The cutoff angle (in degrees)
			\return
		*/
		static VertexBuffer* CreateSpotLight(const Vector3& position, const Vector3& direction, float32 cutoff);

		/*!
			\brief Create a fullscreen [-1, 1] quad 

			\return A vertex buffer containing the fullscreen quad vertices
		*/
		static VertexBuffer* CreateFullscreenQuad();
	};
}
