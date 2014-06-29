#pragma once
#include "../rendering/VertexTypes.h"

namespace core
{
	static const uint32 MAX_BONE_ID_COUNT = 4;

	struct ModelVertexType
	{
		Vector3 pos;
		Vector2 texCoord;
		Vector3 normal;
		uint32 boneID[MAX_BONE_ID_COUNT];
		float32 weight[MAX_BONE_ID_COUNT];
	};

	static const uint32 BONE_ID_ATTRIB_LOCATION = 3;
	static const uint32 BONE_WEIGHT_ATTRIB_LOCATION = 4;

	static const VertexDesc ModelVertexTypeDeclaration = {
		{
			{ POSITION_ATTRIB_LOCATION, sizeof(Vector3), 3, VertexElementType::FLOAT, false },
			{ TEXCOORD_ATTRIB_LOCATION, sizeof(Vector2), 2, VertexElementType::FLOAT, false },
			{ NORMAL_ATTRIB_LOCATION, sizeof(Vector3), 3, VertexElementType::FLOAT, true },
			{ BONE_ID_ATTRIB_LOCATION, sizeof(uint32) * MAX_BONE_ID_COUNT, MAX_BONE_ID_COUNT, VertexElementType::UNSIGNED_INT, false },
			{ BONE_WEIGHT_ATTRIB_LOCATION, sizeof(float32) * MAX_BONE_ID_COUNT, MAX_BONE_ID_COUNT, VertexElementType::FLOAT, false },
			0
		},
		PrimitiveType::TRIANGLE
	};
}
