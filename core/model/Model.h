#pragma once
#include "../resource/ResourceObject.h"
#include "ModelMesh.h"
#include "../collision/AABB.h"
#include "ModelVertexTypeDeclaration.h"
#include <vector>

namespace core
{

	class Model : public ResourceObject
	{
	public:
		//
		// Constructor
		//
		// @param boundingBox
		// @param numMeshes
		Model(const AABB& boundingBox, uint32 numMeshes, ModelMesh* meshes);

		//
		// Destructor
		virtual ~Model();

		//
		// @return This model's bounding box
		inline const AABB& GetBoundingBox() const {
			return mBoundingBox;
		}

		//
		// @return
		inline uint32 GetNumMeshes() const {
			return mNumMeshes;
		}

		//
		// @return
		inline const ModelMesh* GetMeshes() const {
			return mMeshes;
		}

		inline ModelMesh* GetMeshes() {
			return mMeshes;
		}

		inline uint32 GetNumFrames() const {
			return 0;
		}

	private:
		AABB mBoundingBox;
		uint32 mNumMeshes;
		ModelMesh* mMeshes;
	};
}
