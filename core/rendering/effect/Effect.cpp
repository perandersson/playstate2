#include "../../Memory.h"
#include "Effect.h"
#include "../IRenderContext.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../RenderState.h"
using namespace core;

Effect::Effect(GLuint programID)
: ResourceObject(), mProgramID(programID),
mDepthTest(true), mDepthFunc(DepthFunc::DEFAULT),
mStencilTest(false), mStencilMask(BIT_ALL),
mBlend(false), mBlendFunc({ SrcFactor::DEFAULT, DestFactor::DEFAULT }), 
mFrontFace(FrontFace::DEFAULT), mCullFace(CullFace::DEFAULT),
mClearColor(Color::NOTHING), mClearDepth(1.0f)
{
}

Effect::~Effect()
{
	if (mProgramID != 0) {
		glDeleteProgram(mProgramID);
		mProgramID = 0;
	}
}

void Effect::AddUniformProperty(std::shared_ptr<UniformProperty> p)
{
	mProperties.insert(std::make_pair(p->name, p));
}

bool Effect::IsActive() const
{
	RenderState* state = RenderState::GetThreadLocal();
	if (state == nullptr)
		return false;

	return state->IsEffectActive(this);
}
