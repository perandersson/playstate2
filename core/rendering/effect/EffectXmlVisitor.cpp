#include "../../Memory.h"
#include "EffectXmlVisitor.h"
#include "Effect.h"
#include "../../BitUtils.h"
#include "../../xml/TagsAndAttributes.h"
#include "../../StringUtils.h"
#include "../../resource/exception/LoadResourceException.h"
#include "SamplerStateXmlVisitor.h"
#include "../../logging/Logger.h"
#include "../IRenderContext.h"
#include "../RenderState.h"
#include "ShaderObject.h"
#include "../../resource/ResourceManager.h"
#include "../../filesystem/FileUtils.h"
using namespace core;

EffectXmlVisitor::EffectXmlVisitor(IRenderContext* renderContext, const IFile* file)
: XMLDefaultVisitor(), 
mRenderContext(renderContext), mFile(file), mEffect(nullptr), 
mGeometryShaderID(0), mVertexShaderID(0), mFragmentShaderID(0),
mDepthTest(true), mDepthFunc(DepthFunc::DEFAULT), mDepthMask(true),
mStencilTest(false), mStencilMask(BIT_ALL),
mBlend(false), mSrcFactor(SrcFactor::DEFAULT), mDestFactor(DestFactor::DEFAULT),
mFrontFace(FrontFace::DEFAULT), mCullFace(CullFace::DEFAULT)
{

}

EffectXmlVisitor::~EffectXmlVisitor()
{
}

Effect* EffectXmlVisitor::GetEffect()
{
	return mEffect;
}

bool EffectXmlVisitor::VisitEnter(const tinyxml2::XMLElement& element, const tinyxml2::XMLAttribute* firstAttribute)
{
	const std::string name = GetLowerCaseName(element);
	if (name == TAG_BLEND) {
		mBlend = StringUtils::ToBool(GetLowerCaseValue(element));
	} 
	else if (name == TAG_DEPTH_TEST) {
		mDepthTest = StringUtils::ToBool(GetLowerCaseValue(element));
	}
	else if (name == TAG_STENCIL_TEST) {
		mStencilTest = StringUtils::ToBool(GetLowerCaseValue(element));
	}
	else if (name == TAG_STENCIL_MASK) {
		mStencilMask = BitUtils::ToUInt32(GetLowerCaseValue(element));
	}
	else if (name == TAG_FRONT_FACE) {
		mFrontFace = FrontFace::Parse(element.GetText(), FrontFace::DEFAULT);
	}
	else if (name == TAG_CULL_FACE) {
		mCullFace = CullFace::Parse(element.GetText(), CullFace::DEFAULT);
	}
	else if (name == TAG_DEPTH_FUNC) {
		mDepthFunc = DepthFunc::Parse(element.GetText(), DepthFunc::DEFAULT);
	}
	else if (name == TAG_DEPTH_MASK) {
		mDepthMask = StringUtils::ToBool(GetLowerCaseValue(element));
	}
	else if (name == TAG_SRC_FACTOR) {
		mSrcFactor = SrcFactor::Parse(element.GetText(), SrcFactor::DEFAULT);
	}
	else if (name == TAG_DEST_FACTOR) {
		mDestFactor = DestFactor::Parse(element.GetText(), DestFactor::DEFAULT);
	}
	else if (name == TAG_SAMPLER_STATE) {
		const std::string id = GetAttribute(element, ATTRIB_ID);
		const std::string typeName = GetAttribute(element, ATTRIB_TYPE);
		SamplerStateXmlVisitor visitor(id);
		element.Accept(&visitor);
		mProperties.insert(std::make_pair(id, visitor.GetSamplerState()));
		return false;
	}
	else if (name == TAG_GEOMETRY_SHADER) {
		std::string path = GetAttribute(element, ATTRIB_PATH);
		if (path.length() > 0) {
			auto shaderFile = mFile->OpenFile(path);
			Resource<ShaderObject> shader = ResourceManager::GetResource<ShaderObject>(shaderFile);
			if (shader->GetShaderType() != GL_GEOMETRY_SHADER) {
				THROW_EXCEPTION(LoadResourceException, "Invalid fragment shader type. It's not GL_GEOMETRY_SHADER");
			}
			mGeometryShaderID = shader->GetShaderID();
		}
		else {
			const char* text = element.GetText();
			mGeometryShaderID = CompileShader(text, GL_GEOMETRY_SHADER);
			std::string path = FileUtils::GetTempPath(std::string(".gs"));
			ResourceManager::AddResource(new ShaderObject(mGeometryShaderID, GL_GEOMETRY_SHADER), path);
		}
	}
	else if (name == TAG_VERTEX_SHADER) {
		std::string path  = GetAttribute(element, ATTRIB_PATH);
		if (path.length() > 0) {
			auto shaderFile = mFile->OpenFile(path);
			Resource<ShaderObject> shader = ResourceManager::GetResource<ShaderObject>(shaderFile);
			if (shader->GetShaderType() != GL_VERTEX_SHADER) {
				THROW_EXCEPTION(LoadResourceException, "Invalid fragment shader type. It's not GL_VERTEX_SHADER");
			}
			mVertexShaderID = shader->GetShaderID();
		}
		else {
			const char* text = element.GetText();
			mVertexShaderID = CompileShader(text, GL_VERTEX_SHADER);
			std::string path = FileUtils::GetTempPath(std::string(".vs"));
			ResourceManager::AddResource(new ShaderObject(mVertexShaderID, GL_VERTEX_SHADER), path);
		}
	}
	else if (name == TAG_FRAGMENT_SHADER) {
		std::string path = GetAttribute(element, ATTRIB_PATH);
		if (path.length() > 0) {
			auto shaderFile = mFile->OpenFile(path);
			Resource<ShaderObject> shader = ResourceManager::GetResource<ShaderObject>(shaderFile);
			if (shader->GetShaderType() != GL_FRAGMENT_SHADER) {
				THROW_EXCEPTION(LoadResourceException, "Invalid fragment shader type. It's not GL_FRAGMENT_SHADER");
			}
			mFragmentShaderID = shader->GetShaderID();
		}
		else {
			const char* text = element.GetText();
			mFragmentShaderID = CompileShader(text, GL_FRAGMENT_SHADER);
			std::string path = FileUtils::GetTempPath(std::string(".fs"));
			ResourceManager::AddResource(new ShaderObject(mFragmentShaderID, GL_FRAGMENT_SHADER), path);
		}
	}

	return true;
}

bool EffectXmlVisitor::VisitExit(const tinyxml2::XMLElement& element)
{
	const std::string name = GetLowerCaseName(element);
	if (name == TAG_EFFECT) {
		const GLuint program = glCreateProgram();

		if (mGeometryShaderID != 0)
			glAttachShader(program, mGeometryShaderID);
		glAttachShader(program, mVertexShaderID);
		glAttachShader(program, mFragmentShaderID);
		glLinkProgram(program);

		// Detach the shaders when done
		// @see http://www.opengl.org/wiki/GLSL_Object
		glDetachShader(program, mFragmentShaderID);
		mFragmentShaderID = 0;
		glDetachShader(program, mVertexShaderID);
		mVertexShaderID = 0;		
		if (mGeometryShaderID != 0) {
			glDetachShader(program, mGeometryShaderID);
		}

		GLint status = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &status);
		GLchar infoLogg[2048] = { 0 };
		glGetProgramInfoLog(program, sizeof(infoLogg) - 1, NULL, infoLogg);
		if (!status) {
			glDeleteProgram(program);
			THROW_EXCEPTION(LoadResourceException, "Could not link the shaders programs for resource. Reason: %s", infoLogg);
		}

		if (infoLogg != nullptr && strlen(infoLogg) > 40) {
			Logger::Debug("Shader program was linked, and is working, but had build messages:\n%s", infoLogg);
		}

		mEffect = new Effect(program);

		//
		// Prepare the uniform properties
		//

		GLint numUniforms = 0;
		glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numUniforms);
		GLchar nameData[256] = { 0 };
		uint32 nextTextureId = 0;
		for (GLint uniformIndex = 0; uniformIndex < numUniforms; ++uniformIndex) {
			GLint arraySize = 0;
			GLenum type = 0;
			GLsizei actualLength = 0;

			//
			// http://www.opengl.org/sdk/docs/man/xhtml/glGetActiveUniform.xml
			// 

			glGetActiveUniform(program, uniformIndex, sizeof(nameData), &actualLength, &arraySize, &type, nameData);
			nameData[actualLength] = 0;

			const std::string name = nameData;
			const GLint componentID = glGetUniformLocation(program, name.c_str());

			std::shared_ptr<UniformProperty> p;
			UniformProperties::iterator it = mProperties.find(name);
			if (it != mProperties.end()) {
				p = it->second;
			}
			else {
				p = std::shared_ptr<UniformProperty>(new UniformProperty());
				p->name = name;
				mProperties.insert(std::make_pair(name, p));
			}

			p->componentID = componentID;
			p->uniformType = type;
			mEffect->AddUniformProperty(p);
		}
		
		mEffect->SetDepthTest(mDepthTest);
		mEffect->SetDepthFunc(mDepthFunc);
		mEffect->SetDepthMask(mDepthMask);

		mEffect->SetStencilTest(mStencilTest);
		mEffect->SetStencilMask(mStencilMask);

		mEffect->SetBlend(mBlend);
		mEffect->SetBlendFunc(mSrcFactor, mDestFactor);

		mEffect->SetFrontFace(mFrontFace);
		mEffect->SetCullFace(mCullFace);
	}

	return true;
}

GLuint EffectXmlVisitor::CompileShader(const char* text, GLenum type)
{
	GLuint shaderID = glCreateShader(type);
	glShaderSource(shaderID, 1, &text, NULL);
	glCompileShader(shaderID);

	GLint status = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
	if (!status) {
		GLchar infoLogg[2048];
		glGetShaderInfoLog(shaderID, 2048, NULL, infoLogg);
		switch (type) {
		case GL_GEOMETRY_SHADER:
			THROW_EXCEPTION(LoadResourceException, "Could not compile geometry shader. Reason: '%s'", infoLogg);
		case GL_VERTEX_SHADER:
			THROW_EXCEPTION(LoadResourceException, "Could not compile vertex shader. Reason: '%s'", infoLogg);
		case GL_FRAGMENT_SHADER:
			THROW_EXCEPTION(LoadResourceException, "Could not compile fragment shader. Reason: '%s'", infoLogg);
		default:
			THROW_EXCEPTION(LoadResourceException, "Could not compile shader. Reason: '%s'", infoLogg);
		}
	}

	return shaderID;
}
