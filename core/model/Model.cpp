#include "../Memory.h"
#include "Model.h"
using namespace core;

Model::Model(const AABB& boundingBox, uint32 numMeshes, ModelMesh* meshes)
: mBoundingBox(boundingBox), mMeshes(meshes), mNumMeshes(numMeshes)
{
}

Model::~Model()
{
	if (mNumMeshes == 0)
		return;

	for (uint32 i = 0; i < mNumMeshes; ++i) {
		if (mMeshes[i].vertexBuffer != nullptr) {
			delete mMeshes[i].vertexBuffer;
			mMeshes[i].vertexBuffer = nullptr;
		}

		if (mMeshes[i].indexBuffer != nullptr) {
			delete mMeshes[i].indexBuffer;
			mMeshes[i].vertexBuffer = nullptr;
		}
	}

	delete[] mMeshes;
	mMeshes = NULL;
	mNumMeshes = 0;
}
