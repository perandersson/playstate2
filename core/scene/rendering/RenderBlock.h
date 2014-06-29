#pragma once
#include "../../math/matrix4x4.h"
#include "../../math/color.h"

namespace core
{
	class VertexBuffer;
	class IndexBuffer;
	class Texture2D;

	//
	// This block contains all the data neccessary for the pipeline
	// to draw the object onto the screen.
	struct RenderBlock
	{
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