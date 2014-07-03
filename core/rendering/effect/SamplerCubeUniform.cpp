#include "../../Memory.h"
#include "SamplerCubeUniform.h"
#include "Effect.h"
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
	glUniform1i(GetComponentID(), mTextureIndex);
}

void SamplerCubeUniform::SetTexture(const Texture2D* texture)
{
	const uint32 uid = texture != nullptr ? texture->GetUID() : 0;
	if (mTextureUID == uid)
		return;

	mTextureUID = uid;
	mTexture = texture;

	if (IsEffectActive())
		SamplerCubeUniform::Apply();
}

void SamplerCubeUniform::SetTexture(const RenderTarget2D* texture)
{
	SetTexture((Texture2D*)texture);
}

void SamplerCubeUniform::SetTextureParameters(MinFilter::Enum minFilter, MagFilter::Enum magFilter, TextureWrap::Enum wraps, TextureWrap::Enum wrapt)
{
	assert_not_null(mSamplerObject);

	glSamplerParameteri(mSamplerObject->GetSamplerID(), GL_TEXTURE_MIN_FILTER, MinFilter::Parse(minFilter));
	glSamplerParameteri(mSamplerObject->GetSamplerID(), GL_TEXTURE_MAG_FILTER, MagFilter::Parse(magFilter));
	glSamplerParameteri(mSamplerObject->GetSamplerID(), GL_TEXTURE_WRAP_S, TextureWrap::Parse(wraps));
	glSamplerParameteri(mSamplerObject->GetSamplerID(), GL_TEXTURE_WRAP_T, TextureWrap::Parse(wrapt));
}
