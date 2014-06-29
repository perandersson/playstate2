#include "../../Memory.h"
#include "FloatUniform.h"
using namespace core;

FloatUniform::FloatUniform(const Effect* effect, RenderState* state, GLint componentID)
: DefaultUniform(effect, state, componentID), mCount(0)
{
	mValues[0] = 0.0f;
	mValues[1] = 0.0f;
	mValues[2] = 0.0f;
	mValues[3] = 0.0f;
	
	mValuesSet[0] = FLT_MAX;
	mValuesSet[1] = FLT_MAX;
	mValuesSet[2] = FLT_MAX;
	mValuesSet[3] = FLT_MAX;
}

FloatUniform::~FloatUniform()
{
}

void FloatUniform::Apply()
{
	switch(mCount) {
	case 3:
		if(mValuesSet[0] == mValues[0] &&
			mValuesSet[1] == mValues[1] &&
			mValuesSet[2] == mValues[2])
			return;
		mValuesSet[0] = mValues[0];
		mValuesSet[1] = mValues[1];
		mValuesSet[2] = mValues[2];
		glUniform3fv(GetComponentID(), 1, mValues);
		break;
	case 2:
		if(mValuesSet[0] == mValues[0] &&
			mValuesSet[1] == mValues[1])
			return;
		mValuesSet[0] = mValues[0];
		mValuesSet[1] = mValues[1];
		glUniform2fv(GetComponentID(), 1, mValues);
		break;
	case 4:
		if(mValuesSet[0] == mValues[0] &&
			mValuesSet[1] == mValues[1] &&
			mValuesSet[2] == mValues[2] &&
			mValuesSet[3] == mValues[3])
			return;
		mValuesSet[0] = mValues[0];
		mValuesSet[1] = mValues[1];
		mValuesSet[2] = mValues[2];
		mValuesSet[3] = mValues[3];
		glUniform4fv(GetComponentID(), 1, mValues);
		break;
	case 1:
		if(mValuesSet[0] == mValues[0])
			return;
		mValuesSet[0] = mValues[0];
		glUniform1fv(GetComponentID(), 1, mValues);
		break;
	default:
		return;
	}
}

void FloatUniform::SetFloat(float32 a)
{
	mCount = 1;
	mValues[0] = a;

	if (IsEffectActive())
		FloatUniform::Apply();
}

void FloatUniform::SetFloat(float32 a, float32 b)
{
	mCount = 2;
	mValues[0] = a;
	mValues[1] = b;

	if (IsEffectActive())
		FloatUniform::Apply();
}

void FloatUniform::SetFloat(float32 a, float32 b, float32 c)
{
	mCount = 3;
	mValues[0] = a;
	mValues[1] = b;
	mValues[2] = c;

	if (IsEffectActive())
		FloatUniform::Apply();
}

void FloatUniform::SetFloat(float32 a, float32 b, float32 c, float32 d)
{
	mCount = 4;
	mValues[0] = a;
	mValues[1] = b;
	mValues[2] = c;
	mValues[3] = d;

	if (IsEffectActive())
		FloatUniform::Apply();
}

void FloatUniform::SetColorRGBA(const Color& color)
{
	mCount = 4;
	mValues[0] = color.a;
	mValues[1] = color.g;
	mValues[2] = color.b;
	mValues[3] = color.a;

	if (IsEffectActive())
		FloatUniform::Apply();
}

void FloatUniform::SetColorRGB(const Color& color)
{
	mCount = 3;
	mValues[0] = color.r;
	mValues[1] = color.g;
	mValues[2] = color.b;

	if (IsEffectActive())
		FloatUniform::Apply();
}

void FloatUniform::SetVector3(const Vector3& vec)
{
	mCount = 3;
	mValues[0] = vec.x;
	mValues[1] = vec.y;
	mValues[2] = vec.z;

	if (IsEffectActive())
		FloatUniform::Apply();
}

void FloatUniform::SetVector2(const Vector2& vec)
{
	mCount = 2;
	mValues[0] = vec.x;
	mValues[1] = vec.y;

	if (IsEffectActive())
		FloatUniform::Apply();
}
