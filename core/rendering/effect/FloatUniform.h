#pragma once
#include "DefaultUniform.h"
#include "../GLEWMX.h"

namespace core
{
	class FloatUniform : public DefaultUniform
	{
	public:
		FloatUniform(const Effect* effect, RenderState* state, GLint componentID);
		virtual ~FloatUniform();

	// DefaultUniform
	public:
		virtual void Apply();

	// IGfxProgramComponent
	public:
		virtual void SetFloat(float32 a);
		virtual void SetFloat(float32 a, float32 b);
		virtual void SetFloat(float32 a, float32 b, float32 c);
		virtual void SetFloat(float32 a, float32 b, float32 c, float32 d);
		virtual void SetColorRGBA(const Color& color);
		virtual void SetColorRGB(const Color& color);
		virtual void SetVector3(const Vector3& vec);
		virtual void SetVector2(const Vector2& vec);

	private:
		int32 mCount;
		float32 mValues[4];
		float32 mValuesSet[4];
	};

}
