#pragma once
#include "DefaultUniform.h"
#include "../SamplerObject.h"
#include <gl/glew.h>

namespace core
{
	class RenderContext;

	class Sampler2DUniform : public DefaultUniform
	{
	public:
		Sampler2DUniform(const Effect* effect, RenderState* state, GLint componentID, uint32 textureIndex, SamplerObject* samplerObject);
		virtual ~Sampler2DUniform();
		
		virtual void Apply();
		virtual void SetTexture(const Texture2D* texture);
		virtual void SetTexture(const RenderTarget2D* texture);

	private:
		const Texture2D* mTexture;
		uint32 mTextureUID;
		GLuint mTextureIndex;
		SamplerObject* mSamplerObject;
	};
}
