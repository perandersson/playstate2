#pragma once
#include "../../math/Vector3.h"
#include "../../math/Color.h"

namespace core
{
	class Texture2D;
	class VertexBuffer;
	class IndexBuffer;
	class LightSource;
	class Projector;

	/*!
		\brief Struct containing the neccessary parameters to draw the various lights available.
	*/
	struct LightSourceBlock
	{
		/* Position of the spotlight */
		Vector3 position;

		/* The color of the light source */
		Color color;

		/* Constant attenuation for the light source */
		float32 constantAttenuation;

		/* Linear attenuation for the light source */
		float32 linearAttenuation;

		/* Quadric attenuation for the light source */
		float32 quadricAttenuation;

		/* Normalized direction of the spotlight, from the lightsource's point of view */
		Vector3 direction;

		/* SpotLight exponent. Defines how fast the light should fade out */
		float32 spotExponent;

		/* The radius angle, in degrees, for the spotlights and pointlights */
		float32 radius;

		/* Texture, if we want to project a texture onto the screen from the spotlight's point of view*/
		const Texture2D* texture;

		/* The light source vertices, if available */
		const VertexBuffer* vertexBuffer;

		/* The light source indices, if available */
		const IndexBuffer* indexBuffer;

		/* Light projector */
		const Projector* projector;
	};
}
