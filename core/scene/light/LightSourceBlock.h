#pragma once
#include "../../math/Vector3.h"
#include "../../math/Color.h"

namespace core
{
	class Texture2D;
	class VertexBuffer;
	class IndexBuffer;

	struct LightSourceBlock
	{
		Vector3 position;
		Vector3 direction;
		Color color;
		float32 radius;
		float32 spotExponent;
		float32 constantAttenuation;
		float32 linearAttenuation;
		float32 quadricAttenuation;
		const Texture2D* diffuseTexture;
		const VertexBuffer* vertexBuffer;
		const IndexBuffer* indexBuffer;
	};


}
