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
mBlend(false), mBlendFunc({ SrcFactor::DEFAULT, DestFactor::DEFAULT }), mCullFace(CullFace::DEFAULT),
mClearColor(Color::NOTHING), mClearDepth(1.0f),
mDepthRenderTarget(nullptr)
{
	memset(mRenderTargets, 0, sizeof(mRenderTargets));
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

void Effect::SetRenderTarget(RenderTarget2D* renderTarget, uint32 index)
{
	assert(index < MAX_RENDER_TARGETS && "You are not allowed to use that many render targets at once");
	mRenderTargets[index] = renderTarget;
}

void Effect::SetDepthRenderTarget(RenderTarget2D* renderTarget)
{
	mDepthRenderTarget = renderTarget;
}
