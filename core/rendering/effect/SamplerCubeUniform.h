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
		virtual void SetTexture(const Texture2D* texture);
		virtual void SetTexture(const RenderTarget2D* texture);
		virtual void SetTextureParameters(MinFilter::Enum minFilter, MagFilter::Enum magFilter, TextureWrap::Enum wraps, TextureWrap::Enum wrapt);

	private:
		const Texture2D* mTexture;
		uint32 mTextureUID;
		GLuint mTextureIndex;
		SamplerObject* mSamplerObject;
	};
}
