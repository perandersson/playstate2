#pragma once
#include "DefaultUniform.h"
#include <gl/glew.h>

namespace core
{
	class IntUniform : public DefaultUniform
	{
	public:
		IntUniform(const Effect* effect, RenderState* state, GLint componentID);
		virtual ~IntUniform();

	// DefaultUniform
	public:
		virtual void Apply();

	// IGfxProgramComponent
	public:
		virtual void SetInt(int32 a);
		virtual void SetInt(int32 a, int32 b);
		virtual void SetInt(int32 a, int32 b, int32 c);
		virtual void SetInt(int32 a, int32 b, int32 c, int32 d);

	private:
		int32 mCount;
		int32 mValues[4];
		int32 mValuesSet[4];
	};

}
