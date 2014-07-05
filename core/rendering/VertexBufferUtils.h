#pragma once
#include "VertexBuffer.h"
#include "VertexTypes.h"

namespace core
{
	class VertexBufferUtils
	{
	public:
		/*!
			\brief Create VertexBuffer containing a spere with the specified properties

			\param R
					The radius
			\param H
			\param K
			\param Z
			\param usage
			\return
		*/
		static VertexBuffer* CreateSphere(float32 R, float32 H, float32 K, float32 Z, BufferUsage::Enum usage);

		/*!
			\brief Create a fullscreen [-1, 1] quad 

			\return A vertex buffer containing the fullscreen quad vertices
		*/
		static VertexBuffer* CreateFullscreenQuad();
	};
}
