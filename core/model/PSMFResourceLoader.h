#pragma once
#include "../rendering/RenderContextResourceLoader.h"
#include "../resource/Resource.h"
#include "../rendering/VertexBuffer.h"
#include "../rendering/Texture.h"
#include "../math/Color.h"
#include "../collision/AABB.h"
#include "Model.h"
#include <stdio.h>

namespace core
{
	class PSMFResourceLoader : public RenderContextResourceLoader
	{
		struct PSMFMaterial
		{
			std::string name;
			Resource<Texture> diffuseTexture;
			Resource<Texture> ambientTexture;
			Resource<Texture> specularTexture;
			Resource<Texture> specularHighlightTexture;
			Resource<Texture> alphaTexture;
			Resource<Texture> bumpMap;
			Resource<Texture> displacementMap;
			Color ambientColor;
			Color diffuseColor;
			Color specularColor;
			float specularCoefficient;
			float alpha;
		};

		struct PSMFMesh
		{
			std::string material;
			VertexBuffer* vertexBuffer;
		};

		typedef std::hash_map<std::string, PSMFMaterial*> Materials;

	public:
		PSMFResourceLoader();
		virtual ~PSMFResourceLoader();

		// IResourceLoader
	public:
		virtual ResourceObject* Load(const IFile* file);
		virtual ResourceObject* GetDefaultResource();
		virtual bool WatchForFileSystemChanges() const { return false; }

	private:
		ResourceObject* mDefaultResource;
	};
}
