#pragma once
#include "../rendering/RenderContextResourceLoader.h"
#include "../resource/Resource.h"
#include "../rendering/Texture.h"
#include "../math/Color.h"
#include "../collision/AABB.h"
#include "Model.h"
#include <stdio.h>

namespace core
{
	class WavefrontResourceLoader : public RenderContextResourceLoader
	{
		struct WavefrontMaterial
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

		struct WavefrontMesh
		{
			std::string material;
			VertexBuffer* vertexBuffer;
		};

		typedef std::hash_map<std::string, WavefrontMaterial*> Materials;

	public:
		WavefrontResourceLoader();
		virtual ~WavefrontResourceLoader();

	// IResourceLoader
	public:
		virtual ResourceObject* Load(const IFile* file);
		virtual ResourceObject* GetDefaultResource();

	private:
		//
		// Loads the materials from the supplied file and puts the result into the materials hash map
		//
		// @param file 
		//			the file to the materials file.
		// @param materials 
		//			the container map where the materials are to be put.
		void LoadMaterials(const IFile* file, IRenderContext* renderContext, Materials& materials) const;

		//
		// Loads a mesh from an input stream and puts the result into the meshes list.
		void LoadMesh(IRenderContext* renderContext, std::istringstream& stream, std::vector<WavefrontMesh*>& meshes, AABB& boundingBox) const;

	private:
		ResourceObject* mDefaultResource;
	};
}
