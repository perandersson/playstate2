#include "../../Memory.h"
#include "DefaultUniform.h"
#include "../Texture2D.h"
#include "Effect.h"
#include "../RenderState.h"
using namespace core;

DefaultUniform::DefaultUniform(const Effect* effect, RenderState* state, GLint componentID)
: mEffectUID(effect->GetUID()), mRenderState(state), mComponentID(componentID)
{
}

DefaultUniform::~DefaultUniform()
{
}

bool DefaultUniform::IsEffectActive() const
{
	return mRenderState->IsActive(mEffectUID);
}

void DefaultUniform::SetFloat(float32 a)
{
	assert(false && "Invalid component type");
}

void DefaultUniform::SetFloat(float32 a, float32 b)
{
	assert(false && "Invalid component type");
}

void DefaultUniform::SetFloat(float32 a, float32 b, float32 c)
{
	assert(false && "Invalid component type");
}

void DefaultUniform::SetFloat(float32 a, float32 b, float32 c, float32 d)
{
	assert(false && "Invalid component type");
}

void DefaultUniform::SetInt(int32 a)
{
	assert(false && "Invalid component type");
}

void DefaultUniform::SetInt(int32 a, int32 b)
{
	assert(false && "Invalid component type");
}

void DefaultUniform::SetInt(int32 a, int32 b, int32 c)
{
	assert(false && "Invalid component type");
}

void DefaultUniform::SetInt(int32 a, int32 b, int32 c, int32 d)
{
	assert(false && "Invalid component type");
}

void DefaultUniform::SetTexture(const Texture2D* texture)
{
	assert(false && "Invalid component type");
}

void DefaultUniform::SetTexture(Resource<Texture2D>& texture)
{
	SetTexture(texture.Get());
}

void DefaultUniform::SetTexture(const TextureCube* texture)
{
	assert(false && "Invalid component type");
}

void DefaultUniform::SetTexture(Resource<TextureCube>& texture)
{
	SetTexture(texture.Get());
}

void DefaultUniform::SetTexture(const RenderTarget2D* texture)
{
	assert(false && "Invalid component type");
}

void DefaultUniform::SetTexture(const RenderTargetCube* texture)
{
	assert(false && "Invalid component type");
}

void DefaultUniform::SetTextureParameters(MinFilter::Enum minFilter, MagFilter::Enum magFilter, TextureWrap::Enum wraps, TextureWrap::Enum wrapt)
{
	assert(false && "Invalid component type");
}

void DefaultUniform::SetTextureParameters(MinFilter::Enum minFilter, MagFilter::Enum magFilter, TextureWrap::Enum wraps, TextureWrap::Enum wrapt, TextureWrap::Enum wrapr)
{
	assert(false && "Invalid component type");
}

void DefaultUniform::SetMatrix(const Matrix4x4& matrix)
{
	assert(false && "Invalid component type");
}

void DefaultUniform::SetColorRGBA(const Color& color)
{
	assert(false && "Invalid component type");
}

void DefaultUniform::SetColorRGB(const Color& color)
{
	assert(false && "Invalid component type");
}

void DefaultUniform::SetVector3(const Vector3& vec)
{
	assert(false && "Invalid component type");
}

void DefaultUniform::SetVector2(const Vector2& vec)
{
	assert(false && "Invalid component type");
}
