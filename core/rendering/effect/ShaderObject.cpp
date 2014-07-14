#include "ShaderObject.h"
#include "../../Memory.h"
using namespace core;

ShaderObject::ShaderObject(GLuint shaderID, GLenum shaderType)
: mShaderID(shaderID), mShaderType(shaderType)
{

}

ShaderObject::~ShaderObject()
{
	if (mShaderID != 0) {
		glDeleteShader(mShaderID);
		mShaderID = 0;
	}
}
