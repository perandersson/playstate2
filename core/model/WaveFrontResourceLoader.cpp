#include "../Memory.h"
#include "WavefrontResourceLoader.h"
#include "../math/Vector3.h"
#include "../math/Vector2.h"
#include "../filesystem/FileSystem.h"
#include "../resource/ResourceManager.h"
#include "../StringUtils.h"
#include "../rendering/VertexTypes.h"
#include <vector>
using namespace core;

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

WavefrontResourceLoader::WavefrontResourceLoader()
: RenderContextResourceLoader(), mDefaultResource(nullptr)
{
}

WavefrontResourceLoader::~WavefrontResourceLoader()
{
	if (mDefaultResource != nullptr) {
		delete mDefaultResource;
		mDefaultResource = nullptr;
	}
}

ResourceObject* WavefrontResourceLoader::Load(const IFile* file)
{
	std::istringstream dataStream = file->Read();
	std::string path = file->GetAbsolutePath();

	Materials materials;

	AABB boundingBox;
	std::vector<WavefrontMesh*> meshes;

	std::string word;
	while (!dataStream.eof() && (dataStream >> word)) {
		if (word == "mtllib") {
			std::string objectName;
			dataStream >> objectName;
			auto materialsFile = file->OpenFile(objectName);
			if (materialsFile->Exists()) {
				LoadMaterials(materialsFile.get(), GetRenderContext(), materials);
			}
			continue;
		}
		else if (word == "o") {
			std::string objectName;
			dataStream >> objectName;
			LoadMesh(GetRenderContext(), dataStream, meshes, boundingBox);
		}
	}

	ModelMesh* meshesArray = new ModelMesh[meshes.size()];
	unsigned int size = meshes.size();
	for (unsigned int i = 0; i < size; ++i) {
		meshesArray[i].vertexBuffer = meshes[i]->vertexBuffer;
		meshesArray[i].indexBuffer = nullptr;
		meshesArray[i].specularCoefficient = 0.0f;
		meshesArray[i].alpha = 1.0f;

		Materials::iterator it = materials.find(meshes[i]->material);
		if (it != materials.end()) {
			meshesArray[i].diffuseTexture = it->second->diffuseTexture;
			meshesArray[i].diffuseColor = it->second->diffuseColor;
		}

		delete meshes[i];
	}

	// Clean up materials resources - not needed anymore
	Materials::iterator it = materials.begin();
	Materials::const_iterator end = materials.end();
	for (; it != end; ++it) {
		delete it->second;
	}

	Model* model = new Model(boundingBox, meshes.size(), meshesArray);
	return model;
}

void WavefrontResourceLoader::LoadMaterials(const IFile* file, IRenderContext* renderContext, Materials& materials) const
{
	std::istringstream dataStream = file->Read();

	WavefrontMaterial* currentMaterial = NULL;
	std::string word;
	while (!dataStream.eof() && (dataStream >> word)) {
		if (word == "newmtl") {
			if (currentMaterial != NULL)
				materials.insert(std::make_pair(currentMaterial->name, currentMaterial));

			currentMaterial = new WavefrontMaterial();
			currentMaterial->alpha = 1.0f;
			currentMaterial->ambientColor = Color::WHITE;
			currentMaterial->specularColor = Color::WHITE;
			currentMaterial->diffuseColor = Color::WHITE;

			dataStream >> currentMaterial->name;
			continue;
		}
		else if (word == "Ka") {
			dataStream >> currentMaterial->ambientColor.r;
			dataStream >> currentMaterial->ambientColor.g;
			dataStream >> currentMaterial->ambientColor.b;
			continue;
		}
		else if (word == "Kd") {
			dataStream >> currentMaterial->diffuseColor.r;
			dataStream >> currentMaterial->diffuseColor.g;
			dataStream >> currentMaterial->diffuseColor.b;
			continue;
		}
		else if (word == "Ks") {
			dataStream >> currentMaterial->specularColor.r;
			dataStream >> currentMaterial->specularColor.g;
			dataStream >> currentMaterial->specularColor.b;
			continue;
		}
		else if (word == "Ns") {
			dataStream >> currentMaterial->specularCoefficient;
			continue;
		}
		else if ((word == "d") || (word == "Tr")) {
			dataStream >> currentMaterial->alpha;
			continue;
		}
		else if (word == "map_Ka") {
			std::string ambientTexture;
			std::getline(dataStream, ambientTexture);
			if (ambientTexture.length() > 0) {
				currentMaterial->ambientTexture = ResourceManager::GetResource<Texture>(file->OpenFile(ambientTexture));
			}
			continue;
		}
		else if (word == "map_Kd") {
			std::string diffuseTexture;
			dataStream >> diffuseTexture;
			if (diffuseTexture.length() > 0) {
				currentMaterial->diffuseTexture = ResourceManager::GetResource<Texture>(file->OpenFile(diffuseTexture));
			}
			continue;
		}
		else if (word == "map_Ks") {
			std::string specularTexture;
			dataStream >> specularTexture;
			if (specularTexture.length() > 0) {
				currentMaterial->specularTexture = ResourceManager::GetResource<Texture>(file->OpenFile(specularTexture));
			}
			continue;
		}
		else if (word == "map_Ns") {
			std::string specularHighlightTexture;
			std::getline(dataStream, specularHighlightTexture);
			if (specularHighlightTexture.length() > 0) {
				currentMaterial->specularHighlightTexture = ResourceManager::GetResource<Texture>(file->OpenFile(specularHighlightTexture));
			}
			continue;
		}
		else if ((word == "map_d") || (word == "map_Tr")) {
			std::string alphaTexture;
			std::getline(dataStream, alphaTexture);
			if (alphaTexture.length() > 0) {
				currentMaterial->alphaTexture = ResourceManager::GetResource<Texture>(file->OpenFile(alphaTexture));
			}
			continue;
		}
		else if (word == "bump") {
			std::string bumpMap;
			std::getline(dataStream, bumpMap);
			if (bumpMap.length() > 0) {
				currentMaterial->bumpMap = ResourceManager::GetResource<Texture>(file->OpenFile(bumpMap));
			}
			continue;
		}
		else if (word == "disp") {
			std::string displacementMap;
			std::getline(dataStream, displacementMap);
			if (displacementMap.length() > 0) {
				currentMaterial->displacementMap = ResourceManager::GetResource<Texture>(file->OpenFile(displacementMap));
			}
			continue;
		}
		else if (word == "decal") {
			// TODO??
			continue;
		}
	}

	if (currentMaterial != NULL) {
		materials.insert(std::make_pair(currentMaterial->name, currentMaterial));
	}
}

ResourceObject* WavefrontResourceLoader::GetDefaultResource()
{
	if (!mDefaultResource) {
		auto file = FileSystem::OpenFile("/engine/defaults/wavefront_default.obj");
		if (!file->Exists())
			THROW_EXCEPTION(LoadResourceException, "Could not load default resource: '/engine/defaults/wavefront_default.obj'");

		mDefaultResource = WavefrontResourceLoader::Load(file.get());
	}
	return mDefaultResource;
}

void WavefrontResourceLoader::LoadMesh(IRenderContext* renderContext, std::istringstream& stream, std::vector<WavefrontMesh*>& meshes, AABB& boundingBox) const
{
	std::vector<Vector3> vertices;
	std::vector<Vector3> normals;
	std::vector<Vector2> texCoords;
	std::vector<PositionTextureNormalVertexType> items;

	PositionTextureNormalVertexType data[10000];
	PositionTextureNormalVertexType* ptr = data;
	unsigned int numVertices = 0;

	float maxWidth = -FLT_MAX;
	float maxDepth = -FLT_MAX;
	float maxHeight = -FLT_MAX;
	float minWidth = FLT_MAX;
	float minDepth = FLT_MAX;
	float minHeight = FLT_MAX;

	std::string currentMaterial;
	std::string word;
	while (!stream.eof() && (stream >> word)) {
		if (word == "v") {
			Vector3 vertex;
			stream >> vertex.x;
			stream >> vertex.y;
			stream >> vertex.z;
			vertices.push_back(vertex);

			if (vertex.x > maxWidth)
				maxWidth = vertex.x;

			if (vertex.x < minWidth)
				minWidth = vertex.x;

			if (vertex.y > maxHeight)
				maxHeight = vertex.y;

			if (vertex.y < minHeight)
				minHeight = vertex.y;

			if (vertex.z > maxDepth)
				maxDepth = vertex.z;

			if (vertex.z < minDepth)
				minDepth = vertex.z;

		}
		else if (word == "vt") {
			Vector2 texCoord;
			stream >> texCoord.x;
			stream >> texCoord.y;
			texCoords.push_back(texCoord);
		}
		else if (word == "vn") {
			Vector3 normal;
			stream >> normal.x;
			stream >> normal.y;
			stream >> normal.z;
			normals.push_back(normal);
		}
		else if (word == "usemtl") {
			if (currentMaterial.length() > 0) {
				WavefrontMesh* mesh = new WavefrontMesh();
				mesh->material = currentMaterial;
				mesh->vertexBuffer = renderContext->CreateBuffer(data, numVertices, BufferUsage::STATIC);
				meshes.push_back(mesh);

				numVertices = 0;
				ptr = data;
			}

			stream >> currentMaterial;
		}
		else if (word == "f") {
			for (int i = 0; i < 3; ++i) {
				int v = -1, vt = -1, vn = -1;

				std::string face;
				if (!(stream >> face))
					continue;

				std::vector<std::string> parts = StringUtils::Split(face, '/');
				std::stringstream ss(parts[0]);

				ss >> v;
				v--;

				if (parts.size() > 1) {
					ss = std::stringstream(parts[1]);

					ss >> vt;
					vt--;
				}

				if (parts.size() > 2) {
					ss = std::stringstream(parts[2]);

					ss >> vn;
					vn--;
				}

				ptr->pos = vertices[v];
				ptr->texCoord = texCoords[vt];
				ptr->normal = normals[vn];

				ptr++;
				numVertices++;
			}
		}
	}

	if (currentMaterial.length() > 0) {
		WavefrontMesh* mesh = new WavefrontMesh();
		mesh->material = currentMaterial;
		mesh->vertexBuffer = renderContext->CreateBuffer(data, numVertices, BufferUsage::STATIC);
		meshes.push_back(mesh);
	}

	Vector3 mid(((maxWidth - minWidth) / 2.0f) + minWidth,
		((maxHeight - minHeight) / 2.0f) + minHeight,
		((maxDepth - minDepth) / 2.0f) + minDepth);

	boundingBox = AABB(mid, maxWidth - minWidth, maxHeight - minHeight, maxDepth - minDepth);
}


/*
void WavefrontResourceLoader::CreateWavefrontModelMesh(IIVertexBuffer* IVertexBuffer, std::vector<ModelMesh*>& meshes, const WavefrontMaterial* material) const
{
ModelMesh* mesh = new ModelMesh(IVertexBuffer, NULL, material->DiffuseTexture, material->DiffuseColor);
meshes.push_back(mesh);

if(!material->AmbientTexture.empty())
mesh->SetAmbientTexture(mResourceManager.GetResource<ITexture2D>(material->AmbientTexture));

if(!material->SpecularTexture.empty())
mesh->SetSpecularTexture(mResourceManager.GetResource<ITexture2D>(material->SpecularTexture));

if(!material->SpecularHighlightTexture.empty())
mesh->SetSpecularHighlightTexture(mResourceManager.GetResource<ITexture2D>(material->SpecularHighlightTexture));

if(!material->AlphaTexture.empty())
mesh->SetAlphaTexture(mResourceManager.GetResource<ITexture2D>(material->AlphaTexture));

if(!material->BumpMap.empty())
mesh->SetBumpMap(mResourceManager.GetResource<ITexture2D>(material->BumpMap));

if(!material->DisplacementMap.empty())
mesh->SetDisplacementMap(mResourceManager.GetResource<ITexture2D>(material->DisplacementMap));

mesh->SetAmbientColor(material->AmbientColor);
mesh->SetDiffuseColor(material->DiffuseColor);
mesh->SetSpecularColor(material->SpecularColor);
mesh->SetSpecularCoefficient(material->SpecularCoefficient);
mesh->SetAlpha(material->Alpha);

model->AddMesh(mesh);
}
*/
