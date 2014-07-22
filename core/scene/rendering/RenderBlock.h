#pragma once
#include "../../math/matrix4x4.h"
#include "../../math/color.h"

namespace core
{
	class VertexBuffer;
	class IndexBuffer;
	class Texture2D;

	/*!
		\brief Struct containing all the neccessary data for the pipeline to draw it onto the screen.
	*/
	struct RenderBlock
	{
		/* A generated ID based on the non- Renderable unique parameters available for this render block */
		uint32 id;

		const VertexBuffer* vertexBuffer;
		const IndexBuffer* indexBuffer;
		uint32 startIndex;
		uint32 count;
		Matrix4x4 modelMatrix;
		const Texture2D* diffuseTexture;
		Color diffuseColor;
	};


}