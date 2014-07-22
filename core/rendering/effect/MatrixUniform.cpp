#include "../../Memory.h"
#include "MatrixUniform.h"
#include "../exception/RenderingException.h"
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

#if defined(_DEBUG) || defined(RENDERING_TROUBLESHOOTING)
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could assign the matrix4x4 uniform variable");
#endif
}

void MatrixUniform::SetMatrix(const Matrix4x4& matrix)
{
	// The chance of the matrix being the same is so small that we simply ignore to check it!
	mValue = matrix;

	if (IsEffectActive())
		MatrixUniform::Apply();
}
