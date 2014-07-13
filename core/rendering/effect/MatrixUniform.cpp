#include "../../Memory.h"
#include "MatrixUniform.h"
using namespace core;

MatrixUniform::MatrixUniform(const Effect* effect, RenderState* state, GLint componentID)
: DefaultUniform(effect, state, componentID)
{
}

MatrixUniform::~MatrixUniform()
{
}

void MatrixUniform::Apply()
{
	glUniformMatrix4fv(mComponentID, 1, GL_FALSE, mValue._array);
}

void MatrixUniform::SetMatrix(const Matrix4x4& matrix)
{
	// The chance of the matrix being the same is so small that we simply ignore to check it!
	mValue = matrix;

	if (IsEffectActive())
		MatrixUniform::Apply();
}
