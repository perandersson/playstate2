#pragma once
#include "../../math/Vector3.h"
#include "../../math/Color.h"

namespace core
{
	class Texture2D;

	struct LightSourceBlock
	{
		Vector3 position;
		Vector3 direction;
		Color color;
		float32 radius;
		float32 constantAttenuation;
		float32 linearAttenuation;
		float32 quadricAttenuation;
		const Texture2D* diffuseTexture;
	};


}
