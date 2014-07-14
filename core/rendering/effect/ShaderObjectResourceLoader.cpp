#include "../../Memory.h"
#include "ShaderObjectResourceLoader.h"
#include "ShaderObject.h"
using namespace core;

ShaderObjectResourceLoader::ShaderObjectResourceLoader(GLenum type)
: mType(type)
{

}

ShaderObjectResourceLoader::~ShaderObjectResourceLoader()
{

}

ResourceObject* ShaderObjectResourceLoader::Load(const IFile* file)
{
	auto bytes = file->ReadBytes();
	const byte* ptr = bytes.get();
	GLuint shaderID = glCreateShader(mType);
	GLint length = file->GetFileSize();
	glShaderSource(shaderID, 1, (const GLchar**)&ptr, &length);
	glCompileShader(shaderID);

	GLint status = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
	if (!status) {
		GLchar infoLogg[2048];
		glGetShaderInfoLog(shaderID, 2048, NULL, infoLogg);
		glDeleteShader(shaderID);
		const std::string& absolutePath = file->GetAbsolutePath();
		switch (mType) {
		case GL_GEOMETRY_SHADER:
			THROW_EXCEPTION(LoadResourceException, "Could not compile geometry shader: '%s'. Reason: '%s'", absolutePath.c_str(), infoLogg);
		case GL_VERTEX_SHADER:
			THROW_EXCEPTION(LoadResourceException, "Could not compile vertex shader: '%s'. Reason: '%s'", absolutePath.c_str(), infoLogg);
		case GL_FRAGMENT_SHADER:
			THROW_EXCEPTION(LoadResourceException, "Could not compile fragment shader: '%s'. Reason: '%s'", absolutePath.c_str(), infoLogg);
		default:
			THROW_EXCEPTION(LoadResourceException, "Could not compile shader: '%s'. Reason: '%s'", absolutePath.c_str(), infoLogg);
		}
	}

	return new ShaderObject(shaderID, mType);
}

ResourceObject* ShaderObjectResourceLoader::GetDefaultResource()
{
	return nullptr;
}
