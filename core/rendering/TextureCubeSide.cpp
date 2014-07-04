#include "TextureCubeSide.h"
using namespace core;

GLenum TextureCubeSide::Parse(Enum e) {
	static const GLenum textureTargets[TextureCubeSide::SIZE] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
		0, 0
	};

	return textureTargets[(uint32)e];
}

const Vector3& TextureCubeSide::GetDirection(Enum e)
{
	static Vector3 directions[SIZE] = {
		Vector3(1.0f, 0.0f, 0.0f),
		Vector3(-1.0f, 0.0f, 0.0f),
		Vector3(0.0f, 1.0f, 0.0f),
		Vector3(0.0f, -1.0f, 0.0f),
		Vector3(0.0f, 0.0f, 1.0f),
		Vector3(0.0f, 0.0f, -1.0f),
		Vector3(),
		Vector3()
	};

	return directions[(uint32)e];
}
