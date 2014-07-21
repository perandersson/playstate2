#include "../../Memory.h"
#include "Sampler2DUniform.h"
#include "Effect.h"
#include "../OpenGLEnum.h"
#include "../RenderState.h"
using namespace core;

Sampler2DUniform::Sampler2DUniform(const Effect* effect, RenderState* state, GLint componentID, uint32 textureIndex, SamplerObject* samplerObject)
: DefaultUniform(effect, state, componentID), mTextureIndex(textureIndex), mTexture(nullptr), mTextureUID(0), mSamplerObject(samplerObject)
{
}

Sampler2DUniform::~Sampler2DUniform()
{
	if (mSamplerObject != nullptr) {
		delete mSamplerObject;
		mSamplerObject = nullptr;
	}
}

void Sampler2DUniform::Apply()
{
	if(mTexture == nullptr) {
		return;
	}

	mRenderState->BindTexture(mTexture, mTextureIndex);
	mRenderState->BindSampler(mSamplerObject, mTextureIndex);
	glUniform1i(mComponentID, mTextureIndex);
}

void Sampler2DUniform::SetTexture(const Texture2D* texture)
{
	const uint32 uid = texture != nullptr ? texture->GetUID() : 0;
	if (mTextureUID == uid)
		return;

	mTextureUID = uid;
	mTexture = texture;

	if (IsEffectActive())
		Sampler2DUniform::Apply();
}

void Sampler2DUniform::SetTexture(const RenderTarget2D* texture)
{
	SetTexture((Texture2D*)texture);
}

void Sampler2DUniform::SetMinFilter(MinFilter::Enum minFilter)
{
	assert_not_null(mSamplerObject);
	glSamplerParameteri(mSamplerObject->GetSamplerID(), GL_TEXTURE_MIN_FILTER, OpenGLEnum::Convert(minFilter));
}

void Sampler2DUniform::SetMagFilter(MagFilter::Enum magFilter)
{
	assert_not_null(mSamplerObject);
	glSamplerParameteri(mSamplerObject->GetSamplerID(), GL_TEXTURE_MAG_FILTER, OpenGLEnum::Convert(magFilter));
}

void Sampler2DUniform::SetTextureWrap(TextureWrap::Enum s, TextureWrap::Enum t)
{
	assert_not_null(mSamplerObject);
	glSamplerParameteri(mSamplerObject->GetSamplerID(), GL_TEXTURE_WRAP_S, OpenGLEnum::Convert(s));
	glSamplerParameteri(mSamplerObject->GetSamplerID(), GL_TEXTURE_WRAP_T, OpenGLEnum::Convert(t));
}

void Sampler2DUniform::SetTextureCompareFunc(CompareFunc::Enum compareFunc)
{
	assert_not_null(mSamplerObject);

	glSamplerParameteri(mSamplerObject->GetSamplerID(), GL_TEXTURE_COMPARE_FUNC, OpenGLEnum::Convert(compareFunc));
}

void Sampler2DUniform::SetTextureCompareMode(CompareMode::Enum compareMode)
{
	assert_not_null(mSamplerObject);

	glSamplerParameteri(mSamplerObject->GetSamplerID(), GL_TEXTURE_COMPARE_MODE, OpenGLEnum::Convert(compareMode));
}
