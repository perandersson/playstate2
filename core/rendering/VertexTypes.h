#pragma once
#include "../math/Color.h"
#include "../math/Vector2.h"
#include "../math/Vector3.h"
#include "VertexType.h"

namespace core
{
	static const uint32 POSITION_ATTRIB_LOCATION = 0;
	static const uint32 NORMAL_ATTRIB_LOCATION = 1;
	static const uint32 TEXCOORD_ATTRIB_LOCATION = 2;
	static const uint32 COLOR_ATTRIB_LOCATION = 3;

	struct PositionTextureVertexType
	{
		Vector3 pos;
		Vector2 texCoord;

		PositionTextureVertexType()
		{
		}

		PositionTextureVertexType(Vector3 p, Vector2 tex)
		{
			pos = p;
			texCoord = tex;
		}
	};

	static const VertexDesc PositionTextureVertexTypeDeclaration = {
		{
			{ POSITION_ATTRIB_LOCATION, sizeof(Vector3), 3, VertexElementType::FLOAT, false },
			{ TEXCOORD_ATTRIB_LOCATION, sizeof(Vector2), 2, VertexElementType::FLOAT, false },
			0
		},
		PrimitiveType::TRIANGLE
	};

	/////////////////////////////////////////////////////////////////////

	struct PositionTextureNormalVertexType
	{
		Vector3 pos;
		Vector3 normal;
		Vector2 texCoord;

		PositionTextureNormalVertexType()
		{
		}

		PositionTextureNormalVertexType(Vector3 p, Vector2 tex, Vector3 n)
		{
			pos = p;
			texCoord = tex;
			normal = n;
		}
	};

	static const VertexDesc PositionTextureNormalVertexTypeDeclaration = {
		{
			{ POSITION_ATTRIB_LOCATION, sizeof(Vector3), 3, VertexElementType::FLOAT, false },
			{ NORMAL_ATTRIB_LOCATION, sizeof(Vector3), 3, VertexElementType::FLOAT, false },
			{ TEXCOORD_ATTRIB_LOCATION, sizeof(Vector2), 2, VertexElementType::FLOAT, false },
			0
		},
		PrimitiveType::TRIANGLE
	};

}
