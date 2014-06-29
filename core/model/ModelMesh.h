#pragma once
#include "../rendering/VertexBuffer.h"
#include "../rendering/IndexBuffer.h"
#include "../rendering/Texture2D.h"
#include "../resource/Resource.h"
#include "../math/Color.h"

namespace core
{
	struct ModelMesh
	{
		VertexBuffer* vertexBuffer;
		IndexBuffer* indexBuffer;
		Resource<Texture2D> diffuseTexture;
		Resource<Texture2D> ambientTexture;
		Resource<Texture2D> specularTexture;
		Resource<Texture2D> specularHighlightTexture;
		Resource<Texture2D> alphaTexture;
		Resource<Texture2D> bumpMapTexture;
		Resource<Texture2D> displacementTexture;
		Color ambientColor;
		Color diffuseColor;
		Color specularColor;
		float32 specularCoefficient;
		float32 alpha;
	};
}
