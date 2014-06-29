#include "../../Memory.h"
#include "EffectXmlVisitor.h"
#include "Effect.h"
#include "../../xml/TagsAndAttributes.h"
#include "../../StringUtils.h"
#include "../../resource/exception/LoadResourceException.h"
#include "SamplerStateXmlVisitor.h"
#include "../../logging/Logger.h"
#include "../IRenderContext.h"
#include "../RenderState.h"
using namespace core;

EffectXmlVisitor::EffectXmlVisitor(IRenderContext* renderContext)
: XMLDefaultVisitor(), 
mRenderContext(renderContext), mEffect(nullptr), mVertexShaderID(0), mFragmentShaderID(0), mBlend(false), mDepthTest(true), mCullFace(CullFace::DEFAULT),
mDepthFunc(DepthFunc::DEFAULT), mSrcFactor(SrcFactor::DEFAULT), mDestFactor(DestFactor::DEFAULT)
{

}

EffectXmlVisitor::~EffectXmlVisitor()
{
	if (mVertexShaderID != 0) {
		glDeleteShader(mVertexShaderID);
		mVertexShaderID = 0;
	}
	
	if (mFragmentShaderID != 0) {
		glDeleteShader(mFragmentShaderID);
		mFragmentShaderID = 0;
	}
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
	if (name == TAG_DEPTH_TEST) {
		mDepthTest = StringUtils::ToBool(GetLowerCaseValue(element));
	}
	else if (name == TAG_CULL_FACE) {
		mCullFace = CullFace::Parse(element.GetText(), CullFace::DEFAULT);
	}
	else if (name == TAG_DEPTH_FUNC) {
		mDepthFunc = DepthFunc::Parse(element.GetText(), DepthFunc::DEFAULT);
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
	else if (name == TAG_VERTEX_SHADER) {
		const char* text = element.GetText();
		mVertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(mVertexShaderID, 1, &text, NULL);
		glCompileShader(mVertexShaderID);

		GLint status = 0;
		glGetShaderiv(mVertexShaderID, GL_COMPILE_STATUS, &status);
		if (!status) {
			GLchar infoLogg[2048];
			glGetShaderInfoLog(mVertexShaderID, 2048, NULL, infoLogg);
			THROW_EXCEPTION(LoadResourceException, "Could not compile vertex shader: '%s'", infoLogg);
		}
	}
	else if (name == TAG_FRAGMENT_SHADER) {
		mFragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		const char* text = element.GetText();
		glShaderSource(mFragmentShaderID, 1, &text, NULL);
		glCompileShader(mFragmentShaderID);

		GLint status = 0;
		glGetShaderiv(mFragmentShaderID, GL_COMPILE_STATUS, &status);
		if (!status) {
			GLchar infoLogg[2048];
			glGetShaderInfoLog(mFragmentShaderID, 2048, NULL, infoLogg);
			THROW_EXCEPTION(LoadResourceException, "Could not compile fragment shader: '%s'", infoLogg);
		}
	}

	return true;
}

bool EffectXmlVisitor::VisitExit(const tinyxml2::XMLElement& element)
{
	const std::string name = GetLowerCaseName(element);
	if (name == TAG_EFFECT) {
		const GLuint program = glCreateProgram();

		//if (gs != 0)
		//	glAttachShader(program, gs);
		glAttachShader(program, mVertexShaderID);
		glAttachShader(program, mFragmentShaderID);
		glLinkProgram(program);

		// Detach the shaders when done
		// @see http://www.opengl.org/wiki/GLSL_Object
		glDetachShader(program, mFragmentShaderID);
		glDetachShader(program, mVertexShaderID);
		//if (gs != 0) {
		//	glDetachShader(program, gs);
		//	glDeleteShader(gs);
		//}

		GLint status = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &status);
		GLchar infoLogg[2048] = { 0 };
		glGetProgramInfoLog(program, sizeof(infoLogg)-1, NULL, infoLogg);
		if (!status) {
			glDeleteProgram(program);
			THROW_EXCEPTION(LoadResourceException, "Could not link the shaders programs for resource. Reason: %s", infoLogg);
		}
		mEffect = new Effect(program);

		//
		// Prepare the uniform properties
		//

		//glUseProgram(program);

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
		
		mEffect->SetBlend(mBlend);
		mEffect->SetDepthTest(mDepthTest);
		mEffect->SetCullFace(mCullFace);
		mEffect->SetDepthFunc(mDepthFunc);
		mEffect->SetBlendFunc(mSrcFactor, mDestFactor);
	}

	return true;
}
