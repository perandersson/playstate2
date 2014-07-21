#include "../../Memory.h"
#include "SamplerCubeUniform.h"
#include "Effect.h"
#include "../OpenGLEnum.h"
#include "../RenderState.h"
using namespace core;

SamplerCubeUniform::SamplerCubeUniform(const Effect* effect, RenderState* state, GLint componentID, uint32 textureIndex, SamplerObject* samplerObject)
: DefaultUniform(effect, state, componentID), mTextureIndex(textureIndex), mTexture(nullptr), mTextureUID(0), mSamplerObject(samplerObject)
{
}

SamplerCubeUniform::~SamplerCubeUniform()
{
	if (mSamplerObject != nullptr) {
		delete mSamplerObject;
		mSamplerObject = nullptr;
	}
}

void SamplerCubeUniform::Apply()
{
	if(mTexture == nullptr) {
		return;
	}

	mRenderState->BindTexture(mTexture, mTextureIndex);
	mRenderState->BindSampler(mSamplerObject, mTextureIndex);
	glUniform1i(mComponentID, mTextureIndex);
}

void SamplerCubeUniform::SetTexture(const TextureCube* texture)
{
	const uint32 uid = texture != nullptr ? texture->GetUID() : 0;
	if (mTextureUID == uid)
		return;

	mTextureUID = uid;
	mTexture = texture;

	if (IsEffectActive())
		SamplerCubeUniform::Apply();
}

void SamplerCubeUniform::SetTexture(const RenderTargetCube* texture)
{
	SetTexture((TextureCube*)texture);
}

void SamplerCubeUniform::SetMinFilter(MinFilter::Enum minFilter)
{
	assert_not_null(mSamplerObject);
	glSamplerParameteri(mSamplerObject->GetSamplerID(), GL_TEXTURE_MIN_FILTER, OpenGLEnum::Convert(minFilter));
}

void SamplerCubeUniform::SetMagFilter(MagFilter::Enum magFilter)
{
	assert_not_null(mSamplerObject);
	glSamplerParameteri(mSamplerObject->GetSamplerID(), GL_TEXTURE_MAG_FILTER, OpenGLEnum::Convert(magFilter));
}

void SamplerCubeUniform::SetTextureWrap(TextureWrap::Enum s, TextureWrap::Enum t)
{
	assert_not_null(mSamplerObject);
	glSamplerParameteri(mSamplerObject->GetSamplerID(), GL_TEXTURE_WRAP_S, OpenGLEnum::Convert(s));
	glSamplerParameteri(mSamplerObject->GetSamplerID(), GL_TEXTURE_WRAP_T, OpenGLEnum::Convert(t));
}

void SamplerCubeUniform::SetTextureWrap(TextureWrap::Enum s, TextureWrap::Enum t, TextureWrap::Enum r)
{
	assert_not_null(mSamplerObject);
	glSamplerParameteri(mSamplerObject->GetSamplerID(), GL_TEXTURE_WRAP_S, OpenGLEnum::Convert(s));
	glSamplerParameteri(mSamplerObject->GetSamplerID(), GL_TEXTURE_WRAP_T, OpenGLEnum::Convert(t));
	glSamplerParameteri(mSamplerObject->GetSamplerID(), GL_TEXTURE_WRAP_R, OpenGLEnum::Convert(r));
}

void SamplerCubeUniform::SetTextureCompareFunc(CompareFunc::Enum compareFunc)
{
	assert_not_null(mSamplerObject);

	glSamplerParameteri(mSamplerObject->GetSamplerID(), GL_TEXTURE_COMPARE_FUNC, OpenGLEnum::Convert(compareFunc));
}

void SamplerCubeUniform::SetTextureCompareMode(CompareMode::Enum compareMode)
{
	assert_not_null(mSamplerObject);

	glSamplerParameteri(mSamplerObject->GetSamplerID(), GL_TEXTURE_COMPARE_MODE, OpenGLEnum::Convert(compareMode));
}
