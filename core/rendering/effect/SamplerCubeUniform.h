#pragma once
#include "DefaultUniform.h"
#include "../SamplerObject.h"
#include "../GLEWMX.h"

namespace core
{
	class RenderContext;

	class SamplerCubeUniform : public DefaultUniform
	{
	public:
		SamplerCubeUniform(const Effect* effect, RenderState* state, GLint componentID, uint32 textureIndex, SamplerObject* samplerObject);
		virtual ~SamplerCubeUniform();
		
		virtual void Apply();
		virtual void SetTexture(const TextureCube* texture);
		virtual void SetTexture(const RenderTargetCube* texture);
		virtual void SetMinFilter(MinFilter::Enum minFilter);
		virtual void SetMagFilter(MagFilter::Enum magFilter);
		virtual void SetTextureWrap(TextureWrap::Enum s, TextureWrap::Enum t);
		virtual void SetTextureWrap(TextureWrap::Enum s, TextureWrap::Enum t, TextureWrap::Enum r);
		virtual void SetTextureCompareFunc(CompareFunc::Enum compareFunc);
		virtual void SetTextureCompareMode(CompareMode::Enum compareMode);

	private:
		const TextureCube* mTexture;
		uint32 mTextureUID;
		GLuint mTextureIndex;
		SamplerObject* mSamplerObject;
	};
}
