#pragma once
#include "../IUniform.h"

namespace core
{
	class Effect;
	class RenderState;

	class DefaultUniform : public IUniform
	{
	public:
		DefaultUniform(const Effect* effect, RenderState* state, GLint componentID);
		virtual ~DefaultUniform();

		/*!
			\brief Applies this uniform for the currently active effect
		*/
		virtual void Apply() = 0;

		/*!
			\brief Checks if the effect for this uniform is active for the current render state

			\return TRUE if the effect is active; FALSE otherwise
		*/
		bool IsEffectActive() const;

	public:
		virtual void SetFloat(float32 a);
		virtual void SetFloat(float32 a, float32 b);
		virtual void SetFloat(float32 a, float32 b, float32 c);
		virtual void SetFloat(float32 a, float32 b, float32 c, float32 d);

		virtual void SetInt(int32 a);
		virtual void SetInt(int32 a, int32 b);
		virtual void SetInt(int32 a, int32 b, int32 c);
		virtual void SetInt(int32 a, int32 b, int32 c, int32 d);

		virtual void SetTexture(const Texture2D* texture);
		virtual void SetTexture(Resource<Texture2D>& texture);
		virtual void SetTexture(const TextureCube* texture);
		virtual void SetTexture(Resource<TextureCube>& texture);
		virtual void SetTexture(const RenderTarget2D* texture);
		virtual void SetTexture(const RenderTargetCube* texture);

		virtual void SetMinFilter(MinFilter::Enum minFilter);
		virtual void SetMagFilter(MagFilter::Enum magFilter);
		virtual void SetTextureWrap(TextureWrap::Enum s, TextureWrap::Enum t);
		virtual void SetTextureWrap(TextureWrap::Enum s, TextureWrap::Enum t, TextureWrap::Enum r);
		virtual void SetTextureCompareFunc(CompareFunc::Enum compareFunc);
		virtual void SetTextureCompareMode(CompareMode::Enum compareMode);

		virtual void SetColorRGBA(const Color& color);
		virtual void SetColorRGBA(const Color& color, float32 a);
		virtual void SetColorRGB(const Color& color);
		virtual void SetMatrix(const Matrix4x4& matrix);
		virtual void SetVector4(const Vector3& vec, float32 w);
		virtual void SetVector3(const Vector3& vec);
		virtual void SetVector2(const Vector2& vec);

	protected:
		uint32 mEffectUID;
		RenderState* mRenderState;
		GLint mComponentID;
	};
}
