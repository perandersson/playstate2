#include "../Memory.h"
#include "PSMFResourceLoader.h"
#include "../filesystem/FileSystem.h"
#include "../StringUtils.h"
#include "../resource/ResourceManager.h"
#include <algorithm>
using namespace core;

PSMFResourceLoader::PSMFResourceLoader()
: RenderContextResourceLoader(), mDefaultResource(nullptr)
{
}

PSMFResourceLoader::~PSMFResourceLoader()
{
	if (mDefaultResource != nullptr) {
		delete mDefaultResource;
		mDefaultResource = nullptr;
	}
}

ResourceObject* PSMFResourceLoader::Load(const IFile* file)
{
	static const std::string NUM_MATERIALS("NumMaterials %d");
	static const std::string NUM_MESHES("NumMeshes %d");
	static const std::string NUM_POSITIONS("NumPositions %d");
	static const std::string NUM_NORMALS("NumNormals %d");
	static const std::string NUM_TEXCOORDS("NumTexCoords %d");
	static const std::string NUM_TRIANGLES("NumTriangles %d");
	static const std::string MATERIALS("Material \"%[^, \"]%\" {");
	static const std::string MESH("Mesh \"%[^, \"]%\" {");

	//static const std::string JOINTS_LINE("Joints {");
	static const std::string END("}");

	Materials materials;
	std::vector<PositionTextureNormalVertexType> items;
	std::vector<int> indices;

	auto dataStream = file->Read();

	float maxWidth = -FLT_MAX;
	float maxDepth = -FLT_MAX;
	float maxHeight = -FLT_MAX;
	float minWidth = FLT_MAX;
	float minDepth = FLT_MAX;
	float minHeight = FLT_MAX;

	char name[32];
	std::string line;
	int numMeshes = 0;
	ModelMesh* meshes = NULL;
	int currentMesh = 0;
	while (!dataStream.eof()) {
		std::getline(dataStream, line);
		line = StringUtils::RemoveWhitespaces(line);
		int version = 0;
		if (sscanf(line.c_str(), "PSMFVersion %d", &version) == 1) {
			if (version != 1) {
				THROW_EXCEPTION(LoadResourceException, "Invalid model format version. Expected 1 but was %d", version);
			}
		}
		else if (sscanf(line.c_str(), NUM_MESHES.c_str(), &numMeshes) == 1) {
			if (numMeshes == 0 || numMeshes > 100)
				THROW_EXCEPTION(LoadResourceException, "Invalid amount of model meshes count: %d", numMeshes);

			meshes = new ModelMesh[numMeshes];
		}
		//else if (line == JOINTS_LINE) {
		//	while (!dataStream.eof()) {
		//		std::getline(dataStream, line);
		//		line = String::RemoveWhitespaces(line);
		//		if (line == END) {
		//			break;
		//		}

		//		int jointIndex = 0;
		//		char name[32];
		//		int parent = 0;
		//		Vector3 position;
		//		Vector3 orientation;
		//		int optional = 0;
		//		int c = sscanf(line.c_str(), "Joint %d \"%[^, \"]%\" (%d (%f %f %f) (%f %f %f) %d)", &jointIndex, name, &parent, &position.x, &position.y, &position.z,
		//			&orientation.x, &orientation.y, &orientation.z, &optional);
		//		if (c != 10) {
		//			break;
		//		}
		//	}
		//}
		else if (sscanf(line.c_str(), MATERIALS.c_str(), name) == 1) {
			PSMFMaterial* material = new PSMFMaterial();
			material->name = name;

			while (!dataStream.eof()) {
				std::getline(dataStream, line);
				line = StringUtils::RemoveWhitespaces(line);
				if (line == END) {
					break;
				}

				static const std::string DIFFUSE("Diffuse \"%[^, \"]%\" (%f %f %f)");
				
				char pathToFile[256];
				Color color = Color::WHITE;
				if (sscanf(line.c_str(), DIFFUSE.c_str(), pathToFile, &color.r, &color.g, &color.b) == 4) {
					material->diffuseTexture = ResourceManager::GetResource<Texture>(pathToFile);
					material->diffuseColor = color;
				}
			}

			materials.insert(std::make_pair(material->name, material));
		}
		else if (sscanf(line.c_str(), MESH.c_str(), name) == 1) {
			std::vector<Vector3> positions;
			std::vector<Vector3> normals;
			std::vector<Vector2> texCoords;

			while (!dataStream.eof()) {
				std::getline(dataStream, line);
				line = StringUtils::RemoveWhitespaces(line);
				if (line == END) {
					break;
				}

				int count = 0;
				if (sscanf(line.c_str(), NUM_POSITIONS.c_str(), &count) == 1) {
					for (int i = 0; i < count; ++i) {
						std::getline(dataStream, line);
						line = StringUtils::RemoveWhitespaces(line);
						int index = 0;
						Vector3 position;
						if (sscanf(line.c_str(), "Position %d (%f %f %f)", &index, &position.x, &position.y, &position.z) != 4) {
							break;
						}

						positions.push_back(position);

						if (position.x > maxWidth)
							maxWidth = position.x;

						if (position.x < minWidth)
							minWidth = position.x;

						if (position.y > maxHeight)
							maxHeight = position.y;

						if (position.y < minHeight)
							minHeight = position.y;

						if (position.z > maxDepth)
							maxDepth = position.z;

						if (position.z < minDepth)
							minDepth = position.z;
					}
				}
				else if (sscanf(line.c_str(), NUM_NORMALS.c_str(), &count) == 1) {
					for (int i = 0; i < count; ++i) {
						std::getline(dataStream, line);
						line = StringUtils::RemoveWhitespaces(line);
						int index = 0;
						Vector3 normal;
						if (sscanf(line.c_str(), "Normal %d (%f %f %f)", &index, &normal.x, &normal.y, &normal.z) != 4) {
							break;
						}

						normals.push_back(normal);
					}
				}
				else if (sscanf(line.c_str(), NUM_TEXCOORDS.c_str(), &count) == 1) {
					for (int i = 0; i < count; ++i) {
						std::getline(dataStream, line);
						line = StringUtils::RemoveWhitespaces(line);
						int index = 0;
						Vector2 texCoord;
						if (sscanf(line.c_str(), "TexCoord %d (%f %f)", &index, &texCoord.s, &texCoord.t) != 3) {
							break;
						}

						texCoords.push_back(texCoord);
					}
				}
				else if (sscanf(line.c_str(), NUM_TRIANGLES.c_str(), &count) == 1) {
					for (int i = 0; i < count; ++i) {
						std::getline(dataStream, line);
						line = StringUtils::RemoveWhitespaces(line);
						int index = 0;
						int position[3];
						int normal[3];
						int texCoord[3];
						if (sscanf(line.c_str(), "Triangle %d ((%d %d %d) (%d %d %d) (%d %d %d))", &index, &position[0], &normal[0], &texCoord[0], 
							&position[1], &normal[1], &texCoord[1], &position[2], &normal[2], &texCoord[2]) != 10) {
							break;
						}

						//

					}
				}
			}

			currentMesh++;

		}

	}

	Vector3 mid(((maxWidth - minWidth) / 2.0f) + minWidth,
		((maxHeight - minHeight) / 2.0f) + minHeight,
		((maxDepth - minDepth) / 2.0f) + minDepth);
	AABB boundingBox = AABB(mid, maxWidth - minWidth, maxHeight - minHeight, maxDepth - minDepth);

	Model* model = new Model(boundingBox, numMeshes, meshes);
	return model;
}

ResourceObject* PSMFResourceLoader::GetDefaultResource()
{
	if (!mDefaultResource) {
		auto file = FileSystem::OpenFile("/engine/defaults/model.psmf");
		if (!file->Exists())
			THROW_EXCEPTION(LoadResourceException, "Could not load default resource: '%s'", file->GetAbsolutePath().c_str());

		mDefaultResource = PSMFResourceLoader::Load(file.get());
	}
	return mDefaultResource;
}
