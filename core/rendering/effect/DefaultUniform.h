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

		//
		// Apply this uniform
		virtual void Apply() = 0;

		//
		// @return TRUE if the owner effect of this uniform is active; FALSE otherwise
		bool IsEffectActive() const;

		//
		// 
		inline GLint GetComponentID() const {
			return mComponentID;
		}

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
		virtual void SetTexture(const RenderTarget2D* texture);

		virtual void SetColorRGBA(const Color& color);
		virtual void SetColorRGB(const Color& color);
		virtual void SetMatrix(const Matrix4x4& matrix);
		virtual void SetVector3(const Vector3& vec);
		virtual void SetVector2(const Vector2& vec);

	protected:
		uint32 mEffectUID;
		RenderState* mRenderState;
		GLint mComponentID;
	};
}
