#pragma once
#include "../typedefs.h"
#include "Texture2D.h"
#include "TextureCube.h"
#include "TextureCubeSide.h"
#include "RenderTarget2D.h"
#include "RenderTargetCube.h"
#include "../resource/Resource.h"
#include "../math/Color.h"
#include "../math/Matrix4x4.h"
#include "../math/Vector3.h"
#include "../math/Vector2.h"
#include "MinFilter.h"
#include "MagFilter.h"
#include "TextureWrap.h"
#include "CompareFunc.h"
#include "CompareMode.h"

namespace core
{
	class IUniform
	{
	public:
		virtual ~IUniform() {}

		virtual void SetFloat(float32 a) = 0;
		virtual void SetFloat(float32 a, float32 b) = 0;
		virtual void SetFloat(float32 a, float32 b, float32 c) = 0;
		virtual void SetFloat(float32 a, float32 b, float32 c, float32 d) = 0;

		virtual void SetInt(int32 a) = 0;
		virtual void SetInt(int32 a, int32 b) = 0;
		virtual void SetInt(int32 a, int32 b, int32 c) = 0;
		virtual void SetInt(int32 a, int32 b, int32 c, int32 d) = 0;

		virtual void SetTexture(const Texture2D* texture) = 0;
		virtual void SetTexture(Resource<Texture2D>& texture) = 0;
		virtual void SetTexture(const TextureCube* texture) = 0;
		virtual void SetTexture(Resource<TextureCube>& texture) = 0;
		virtual void SetTexture(const RenderTarget2D* texture) = 0;
		virtual void SetTexture(const RenderTargetCube* texture) = 0;

		virtual void SetMinFilter(MinFilter::Enum minFilter) = 0;
		virtual void SetMagFilter(MagFilter::Enum magFilter) = 0;

		virtual void SetTextureWrap(TextureWrap::Enum s, TextureWrap::Enum t) = 0;
		virtual void SetTextureWrap(TextureWrap::Enum s, TextureWrap::Enum t, TextureWrap::Enum r) = 0;

		virtual void SetTextureCompareFunc(CompareFunc::Enum compareFunc) = 0;
		virtual void SetTextureCompareMode(CompareMode::Enum compareMode) = 0;

		virtual void SetColorRGBA(const Color& color) = 0;
		virtual void SetColorRGBA(const Color& color, float32 a) = 0;
		virtual void SetColorRGB(const Color& color) = 0;
		virtual void SetMatrix(const Matrix4x4& matrix) = 0;
		virtual void SetVector4(const Vector3& vec, float32 w) = 0;
		virtual void SetVector3(const Vector3& vec) = 0;
		virtual void SetVector2(const Vector2& vec) = 0;
	};
}
