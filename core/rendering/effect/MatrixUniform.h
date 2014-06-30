#pragma once
#include "DefaultUniform.h"
#include "../GLEWMX.h"

namespace core
{
	class MatrixUniform : public DefaultUniform
	{
	public:
		MatrixUniform(const Effect* effect, RenderState* state, GLint componentID);
		virtual ~MatrixUniform();

	// DefaultUniform
	public:
		virtual void Apply();

	// IGfxProgramComponent
	public:
		virtual void SetMatrix(const Matrix4x4& matrix);

	private:
		Matrix4x4 mValue;
	};

}
