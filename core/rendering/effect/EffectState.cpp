#include "../../Memory.h"
#include "EffectState.h"
#include "../RenderState.h"
#include "Effect.h"
#include "UniformNotFound.h"
#include "DefaultUniform.h"
#include "FloatUniform.h"
#include "IntUniform.h"
#include "MatrixUniform.h"
#include "Sampler2DUniform.h"
#include "../exception/RenderingException.h"
using namespace core;

EffectState::EffectState(const Effect* effect, RenderState* renderState)
: mEffect(effect)
{
	auto& properties = effect->GetUniformProperties();
	auto it = properties.begin();
	auto end = properties.end();
	for (; it != end; ++it) {
		DefaultUniform* uniform = nullptr;
		auto* prpt = it->second.get();

		const GLuint program = effect->GetProgramID();
		const GLint componentID = glGetUniformLocation(program, prpt->name.c_str());

		if (prpt->uniformType == GL_FLOAT || prpt->uniformType == GL_FLOAT_VEC2 || prpt->uniformType == GL_FLOAT_VEC3 || prpt->uniformType == GL_FLOAT_VEC4) {
			uniform = new FloatUniform(effect, renderState, componentID);
		}
		else if (prpt->uniformType == GL_INT || prpt->uniformType == GL_INT_VEC2 || prpt->uniformType == GL_INT_VEC3 || prpt->uniformType == GL_INT_VEC4) {
			uniform = new IntUniform(effect, renderState, componentID);
		}
		else if (prpt->uniformType == GL_UNSIGNED_INT || prpt->uniformType == GL_UNSIGNED_INT_VEC2 || prpt->uniformType == GL_UNSIGNED_INT_VEC3 || prpt->uniformType == GL_UNSIGNED_INT_VEC4) {
			uniform = new IntUniform(effect, renderState, componentID);
		}
		else if (prpt->uniformType == GL_FLOAT_MAT4) {
			uniform = new MatrixUniform(effect, renderState, componentID);
		}
		else if (prpt->uniformType == GL_SAMPLER_2D) {
			uniform = new Sampler2DUniform(effect, renderState, componentID, renderState->GetNextTextureIndex(), GenSamplerObject(it->second));
		}

		if (uniform == nullptr)
			THROW_EXCEPTION(RenderingException, "Unknown uniform \"%s\" of type: %d", it->second, prpt->uniformType);
			
		mUniforms.insert(std::make_pair(it->first, uniform));
	}
}

EffectState::~EffectState()
{
	auto it = mUniforms.begin();
	auto end = mUniforms.end();
	for (; it != end; ++it) {
		delete it->second;
	}
}

void EffectState::ApplyUniforms()
{
	auto it = mUniforms.begin();
	auto end = mUniforms.end();
	for (; it != end; ++it) {
		it->second->Apply();
	}
}

IUniform* EffectState::FindUniform(const char* name)
{
	return FindUniform(std::string(name));
}

IUniform* EffectState::FindUniform(const std::string& name)
{
	IUniform* uniform = NULL;
	auto it = mUniforms.find(name);
	if (it == mUniforms.end()) {
		static UniformNotFound uniformNotFound;
		return &uniformNotFound;
	}
	return it->second;
}

SamplerObject* EffectState::GenSamplerObject(std::shared_ptr<UniformProperty> property)
{
	GLuint samplerID = 0;
	glGenSamplers(1, &samplerID);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could create a new SamplerObject from effect file. Reason: %d", error);

	SamplerObject* samplerObject = new SamplerObject(samplerID, property->minFilter, property->magFilter, property->wraps, property->wrapt);

	glSamplerParameteri(samplerID, GL_TEXTURE_MIN_FILTER, MinFilter::Parse(property->minFilter));
	glSamplerParameteri(samplerID, GL_TEXTURE_MAG_FILTER, MagFilter::Parse(property->magFilter));
	glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_S, TextureWrap::Parse(property->wraps));
	glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_T, TextureWrap::Parse(property->wrapt));

	error = glGetError();
	if (error != GL_NO_ERROR) {
		THROW_EXCEPTION(RenderingException, "Could create a bind states to SamplerObject. Reason: %d", error);
	}

	return samplerObject;
}