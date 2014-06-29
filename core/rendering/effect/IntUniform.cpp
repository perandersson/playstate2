#include "../../Memory.h"
#include "IntUniform.h"
using namespace core;

IntUniform::IntUniform(const Effect* effect, RenderState* state, GLint componentID)
: DefaultUniform(effect, state, componentID), mCount(0)
{
	mValues[0] = 0;
	mValues[1] = 0;
	mValues[2] = 0;
	mValues[3] = 0;

	mValuesSet[0] = INT_MAX;
	mValuesSet[1] = INT_MAX;
	mValuesSet[2] = INT_MAX;
	mValuesSet[3] = INT_MAX;
}

IntUniform::~IntUniform()
{
}

void IntUniform::Apply()
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
		glUniform3iv(GetComponentID(), 1, mValues);
		break;
	case 2:
		if(mValuesSet[0] == mValues[0] &&
			mValuesSet[1] == mValues[1])
			return;
		mValuesSet[0] = mValues[0];
		mValuesSet[1] = mValues[1];
		glUniform3iv(GetComponentID(), 1, mValues);
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
		glUniform3iv(GetComponentID(), 1, mValues);
		break;
	case 1:
		if(mValuesSet[0] == mValues[0])
			return;
		mValuesSet[0] = mValues[0];
		glUniform3iv(GetComponentID(), 1, mValues);
		break;
	default:
		return;
	}
}

void IntUniform::SetInt(int32 a)
{
	mCount = 1;
	mValues[0] = a;

	if (IsEffectActive())
		IntUniform::Apply();
}

void IntUniform::SetInt(int32 a, int32 b)
{
	mCount = 2;
	mValues[0] = a;
	mValues[1] = b;

	if (IsEffectActive())
		IntUniform::Apply();
}

void IntUniform::SetInt(int32 a, int32 b, int32 c)
{
	mCount = 3;
	mValues[0] = a;
	mValues[1] = b;
	mValues[2] = c;

	if (IsEffectActive())
		IntUniform::Apply();
}

void IntUniform::SetInt(int32 a, int32 b, int32 c, int32 d)
{
	mCount = 4;
	mValues[0] = a;
	mValues[1] = b;
	mValues[2] = c;
	mValues[3] = d;

	if (IsEffectActive())
		IntUniform::Apply();
}
