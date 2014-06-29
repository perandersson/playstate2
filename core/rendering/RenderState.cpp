#include "../Memory.h"
#include "RenderState.h"
#include "RenderTarget2D.h"
#include "SamplerObject.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "effect/Effect.h"
#include "exception/RenderingException.h"
using namespace core;

template<> RenderState* ThreadLocal<RenderState*>::gThreadLocal = nullptr;

RenderState::RenderState()
: mViewport(Rect::ZERO), 
mEffectUID(0),
mVertexBufferUID(0), mIndexBufferUID(0), mVertexArrayID(0),
mDepthTest(false), mDepthFunc(DepthFunc::DEFAULT),
mBlend(false), mBlendFunc({ SrcFactor::DEFAULT, DestFactor::DEFAULT }),
mCullFace(CullFace::DEFAULT),
mClearColor(Color::NOTHING), mClearDepth(1.0f),
mActiveTextureIndex(0),
mDepthRenderTarget(nullptr), mDepthRenderTargetType(GL_DEPTH_ATTACHMENT), mDepthRenderTargetUID(0), mFrameBufferID(0), mApplyRenderTarget(false), mFrameBufferApplied(false),
mNextTextureIndex(0), mMaxTextureIndexes(0), mEffectApplied(false)
{
	memset(mTextureUID, 0, sizeof(mTextureUID));
	memset(mTextureTarget, 0, sizeof(mTextureTarget));
	memset(mRenderTargetUID, 0, sizeof(mRenderTargetUID));
	memset(mSamplerObjectUID, 0, sizeof(mSamplerObjectUID));
	memset(mRenderTargets, 0, sizeof(mRenderTargets));

	glClearColor(mClearColor.r, mClearColor.g, mClearColor.b, mClearColor.a);
	glClearDepth(mClearDepth);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ZERO);
	glActiveTexture(GL_TEXTURE0);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Coult not initialize this thread's rendering states");

	mMaxTextureIndexes = MAX_ACTIVE_TEXTURES; // TODO: Ask OpenGL about this
}

RenderState::~RenderState()
{
	if (mFrameBufferID != 0) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &mFrameBufferID);
		mFrameBufferID = 0;
	}

	if (mVertexArrayID != 0) {
		glDeleteVertexArrays(1, &mVertexArrayID);
		mVertexArrayID = 0;
	}
}

void RenderState::Clear(uint32 clearBits)
{
	if (mApplyRenderTarget) {
		mApplyRenderTarget = false;
		ApplyRenderTargets();
	}

	GLenum clear = 0;
	if (BIT_ISSET(clearBits, ClearType::COLOR)) {
		clear = GL_COLOR_BUFFER_BIT;
	}

	if (BIT_ISSET(clearBits, ClearType::DEPTH)) {
		clear |= GL_DEPTH_BUFFER_BIT;
	}

	SetClearDepth(mClearDepth);
	SetClearColor(mClearColor);

	if (clear != 0)
		glClear(clear);

	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not clear the screen");
}

EffectState* RenderState::BindEffect(const Effect* effect)
{
	assert_not_null(effect);
	const uint32 uid = effect->GetUID();
	if (uid == mEffectUID)
		return mEffectState;

	// Find the appropriate effect state
	mEffectState = GetEffectState(effect);
	mEffectApplied = false;

	glUseProgram(effect->GetProgramID());
	mEffectUID = uid;

	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not change which program to use whenever render faces on the screen");

	//
	// Set this render state's properties based on the supplied effect
	//

	SetDepthTest(effect->GetDepthTest());
	SetDepthFunc(effect->GetDepthFunc());
	SetBlend(effect->GetBlend());

	auto blendFunc = effect->GetBlendFunc();
	SetBlendFunc(blendFunc.sfactor, blendFunc.dfactor);

	SetCullFace(effect->GetCullFace());
	SetClearColor(effect->GetClearColor());
	SetClearDepth(effect->GetClearDepth());

	const RenderTarget2D* const* renderTargets = effect->GetRenderTargets();
	for (uint32 i = 0; i < MAX_RENDER_TARGETS; ++i)
		SetRenderTarget(renderTargets[i], i);

	SetDepthRenderTarget(effect->GetDepthRenderTarget());
	ApplyRenderTargets();

	// Apply the effects various uniform variables
	//mEffectState->ApplyUniforms();
	//mEffectApplied = true;

	return mEffectState;
}

void RenderState::Render(const VertexBuffer* buffer)
{
	assert_not_null(buffer);

	ApplyBuffers(buffer, nullptr);
	Render(buffer, nullptr, 0);
}

void RenderState::Render(const VertexBuffer* buffer, const IndexBuffer* indexBuffer)
{
	assert_not_null(buffer);

	ApplyBuffers(buffer, indexBuffer);
	Render(buffer, indexBuffer, 0);
}

void RenderState::Render(const VertexBuffer* buffer, const IndexBuffer* indexBuffer, uint32 startIndex)
{
	assert_not_null(buffer);

	ApplyBuffers(buffer, indexBuffer);

	if (indexBuffer != nullptr)
		buffer->Render(indexBuffer, startIndex);
	else
		buffer->Render(startIndex);

	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not render index and vertex buffers");

}

void RenderState::Render(const VertexBuffer* buffer, const IndexBuffer* indexBuffer, uint32 startIndex, uint32 numVertices)
{
	assert_not_null(buffer);
	assert(numVertices > 0 && "You cannot draw 0 vertices");

	ApplyBuffers(buffer, indexBuffer);

	if (indexBuffer != nullptr)
		buffer->Render(indexBuffer, startIndex, numVertices);
	else
		buffer->Render(startIndex, numVertices);
}

void RenderState::BindVertexBuffer(const VertexBuffer* vertexBuffer)
{
	assert_not_null(vertexBuffer);
	const uint32 uid = vertexBuffer->GetUID();
	if (mVertexBufferUID == uid)
		return;

	if (mVertexArrayID == 0) {
		glGenVertexArrays(1, &mVertexArrayID);
		glBindVertexArray(mVertexArrayID);

		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
			THROW_EXCEPTION(RenderingException, "Could not bind the newly vertex array");
	}

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->GetBufferID());

	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not bind the supplied vertex buffer");

	//
	// Calculate the object's stride
	//

	uint32 stride = 0;
	const VertexDesc& desc = vertexBuffer->GetVertexDesc();
	for (uint32 i = 0; i < 8; ++i) {
		const VertexElementDesc& elemDesc = desc.elements[i];
		if (elemDesc.fieldSize == 0)
			break;

		stride += elemDesc.fieldSize;
	}

	static const GLenum enums[VertexElementType::SIZE] = {
		GL_BYTE,
		GL_UNSIGNED_BYTE,
		GL_SHORT,
		GL_UNSIGNED_SHORT,
		GL_INT,
		GL_UNSIGNED_INT,
		GL_FLOAT,
		GL_DOUBLE
	};

	//
	// Set the vertex attributes
	//

	uint32 offset = 0;
	for (uint32 i = 0; i < 8; ++i) {
		const VertexElementDesc& elemDesc = desc.elements[i];
		if (elemDesc.fieldSize == 0)
			break;

		glEnableVertexAttribArray(elemDesc.location);
		if (HandleAsIntegerType(elemDesc)) {
			glVertexAttribIPointer(elemDesc.location, elemDesc.numElementsInField, enums[elemDesc.type], stride, OFFSET(offset));
		}
		else {
			glVertexAttribPointer(elemDesc.location, elemDesc.numElementsInField, enums[elemDesc.type], elemDesc.normalized ? GL_TRUE : GL_FALSE, stride, OFFSET(offset));
		}
		offset += elemDesc.fieldSize;
	}

	mVertexBufferUID = uid;
}

void RenderState::BindIndexBuffer(const IndexBuffer* indexBuffer)
{
	const uint32 uid = indexBuffer != nullptr ? indexBuffer->GetUID() : 0;
	if (mIndexBufferUID == uid)
		return;

	const GLuint bufferID = indexBuffer == nullptr ? indexBuffer->GetBufferID() : 0;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
	mIndexBufferUID = uid;

	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not bind the supplied index buffer");
}

void RenderState::SetViewport(const Rect& viewport)
{
	if (mViewport != viewport) {
		glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
		mViewport = viewport;
	}

	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not set the screen viewport");
}

void RenderState::SetDepthTest(bool enable)
{
	if (mDepthTest == enable)
		return;

	if (enable)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
	mDepthTest = enable;

	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not enable/disable depth testing");
}

void RenderState::SetDepthFunc(DepthFunc::Enum func)
{
	if (mDepthFunc == func)
		return;

	glDepthFunc(GetDepthFuncAsEnum(func));
	mDepthFunc = func;

	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not set the depth function used when render faces on the screen");
}

void RenderState::SetBlend(bool enable)
{
	if (mBlend == enable)
		return;

	if (enable)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);
	mBlend = enable;

	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not enable/disable blending");
}

void RenderState::SetBlendFunc(SrcFactor::Enum sfactor, DestFactor::Enum dfactor)
{
	if (mBlendFunc.sfactor == sfactor && mBlendFunc.dfactor == dfactor)
		return;

	glBlendFunc(GetSrcFactorAsEnum(sfactor), GetDestFactorAsEnum(dfactor));
	mBlendFunc.sfactor = sfactor;
	mBlendFunc.dfactor = dfactor;

	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not change the blend functions the cull faces used when render faces on the screen");
}

void RenderState::SetCullFace(CullFace::Enum cullFace)
{
	if (mCullFace == cullFace)
		return;

	if (cullFace == CullFace::DISABLED)
		glDisable(GL_CULL_FACE);
	else
	{
		glEnable(GL_CULL_FACE);
		glFrontFace(GetCullFaceAsEnum(cullFace));
		glCullFace(GL_BACK);
	}
	mCullFace = cullFace;

	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not change the cull faces used when render faces on the screen");
}

void RenderState::SetClearColor(const Color& color)
{
	if (mClearColor == color)
		return;

	glClearColor(color.r, color.g, color.b, color.a);
	mClearColor = color;

	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not set the colors used whenever clearing the screen");
}

void RenderState::SetClearDepth(float32 depth)
{
	if (FLOAT_EQUALS(depth, mClearDepth))
		return;

	glClearDepth(depth);
	mClearDepth = depth;

	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not change the clearing depth");
}

void RenderState::BindTexture(const Texture* texture, uint32 index)
{
	assert(index < MAX_ACTIVE_TEXTURES && "You are not allowed to bind that many textures");
	if (mActiveTextureIndex != index) {
		glActiveTexture(GL_TEXTURE0 + index);
		mActiveTextureIndex = index;

		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
			THROW_EXCEPTION(RenderingException, "Could not activate texture index: %d", index);
	}

	const uint32 uid = texture != nullptr ? texture->GetUID() : 0;
	if (mTextureUID[index] == uid)
		return;

	const GLuint textureID = texture != nullptr ? texture->GetTextureID() : 0;
	const GLenum textureTarget = texture != nullptr ? texture->GetTextureTarget() : mTextureTarget[index];
	glBindTexture(textureTarget, textureID);

	mTextureUID[index] = uid;
	mTextureTarget[index] = textureTarget;

	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not bind texture");
}

void RenderState::BindSampler(SamplerObject* samplerObject, uint32 index)
{
	const uint32 uid = samplerObject != nullptr ? samplerObject->GetUID() : 0;
	if (mSamplerObjectUID[index] == uid)
		return;

	const GLuint samplerID = samplerObject != nullptr ? samplerObject->GetSamplerID() : 0;
	glBindSampler(index, samplerID);
	mSamplerObjectUID[index] = uid;

	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not bind the texture sampler object");
}

void RenderState::SetRenderTarget(const RenderTarget2D* renderTarget, GLenum index)
{
	assert(index < MAX_RENDER_TARGETS && "You are not allowed to bind that many render targets");
	const uint32 uid = renderTarget != nullptr ? renderTarget->GetUID() : 0;
	if (mRenderTargetUID[index] == uid)
		return;

	mRenderTargets[index] = renderTarget;
	mApplyRenderTarget = true;
}

void RenderState::SetDepthRenderTarget(const RenderTarget2D* renderTarget)
{
	const uint32 uid = renderTarget != nullptr ? renderTarget->GetUID() : 0;
	if (mDepthRenderTargetUID == uid)
		return;

	mDepthRenderTarget = renderTarget;
	mApplyRenderTarget = true;
}

IUniform* RenderState::FindUniform(const char* name)
{
	assert_not_null(mEffectState);
	return mEffectState->FindUniform(name);
}

IUniform* RenderState::FindUniform(const std::string& name)
{
	assert_not_null(mEffectState);
	return mEffectState->FindUniform(name);
}

bool RenderState::IsRenderTargetsEnabled() const
{
	if (mDepthRenderTarget != nullptr)
		return true;

	for (int i = 0; i < MAX_RENDER_TARGETS; ++i)
		if (mRenderTargets[i] != nullptr)
			return true;

	return false;
}

void RenderState::ApplyRenderTargets()
{
	// Disable frame buffer if neccessary
	if (!IsRenderTargetsEnabled()) {
		if (mFrameBufferApplied) {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			mFrameBufferApplied = false;

			memset(mRenderTargetUID, 0, sizeof(mRenderTargetUID));
			memset(mRenderTargets, 0, sizeof(mRenderTargets));
			mDepthRenderTarget = 0;
			mDepthRenderTargetUID = 0;

			GLenum err = glGetError();
			if (err != GL_NO_ERROR)
				THROW_EXCEPTION(RenderingException, "Could not unbind frame buffer object. Reason: %d", err);
		}

		return;
	}

	// Create frame buffer if exists
	if (mFrameBufferID == 0) {
		glGenFramebuffers(1, &mFrameBufferID);
		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
			THROW_EXCEPTION(RenderingException, "Could not create frame buffer object. Reason: %d", err);
	}

	// Bind frame buffer if it's not already bound to this render context
	if (!mFrameBufferApplied) {
		glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);
		mFrameBufferApplied = true;

		// Check for any GL errors
		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
			THROW_EXCEPTION(RenderingException, "Could not bind frame buffer");
	}
	
	Size size;
	const uint32 depthUID = mDepthRenderTarget != nullptr ? mDepthRenderTarget->GetUID() : 0;
	if (mDepthRenderTargetUID != depthUID) {
		if (mDepthRenderTarget != nullptr) {
			GLenum attachmentType = GL_DEPTH_ATTACHMENT;
			if (mDepthRenderTarget->GetTextureFormat() == TextureFormat::DEPTH24_STENCIL8)
				attachmentType = GL_DEPTH_STENCIL_ATTACHMENT;

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, mDepthRenderTarget->GetTextureID(), 0);

			GLenum err = glGetError();
			if (err != GL_NO_ERROR)
				THROW_EXCEPTION(RenderingException, "Could not attach depth render target to frame buffer. Reason: %d", err);

			mDepthRenderTargetType = attachmentType;
		}
		else {
			glFramebufferTexture2D(GL_FRAMEBUFFER, mDepthRenderTargetType, GL_TEXTURE_2D, 0, 0);

			GLenum err = glGetError();
			if (err != GL_NO_ERROR)
				THROW_EXCEPTION(RenderingException, "Could not detach depth render target from the frame buffer. Reason: %d", err);
		}
		mDepthRenderTargetUID = depthUID;
	}


	GLenum drawBuffers[MAX_RENDER_TARGETS] = { 0 };
	uint32 numDrawBuffers = 0;
	for (int i = 0; i < MAX_RENDER_TARGETS; ++i) {
		const RenderTarget2D* rt = mRenderTargets[i];
		const uint32 uid = rt != nullptr ? rt->GetUID() : 0;
		if (uid == mRenderTargetUID[i]) {
			if (rt != nullptr) {
				size = rt->GetSize();
				drawBuffers[numDrawBuffers++] = GL_COLOR_ATTACHMENT0 + i;
			}
			continue;
		}

		if (rt != nullptr) {
			size = rt->GetSize();
			drawBuffers[numDrawBuffers++] = GL_COLOR_ATTACHMENT0 + i;
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, rt->GetTextureID(), 0);

			GLenum err = glGetError();
			if (err != GL_NO_ERROR)
				THROW_EXCEPTION(RenderingException, "Could not attach render target to frame buffer. Reason: %d", err);
		}
		else {
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, 0, 0);

			GLenum err = glGetError();
			if (err != GL_NO_ERROR)
				THROW_EXCEPTION(RenderingException, "Could not detach render target from the frame buffer. Reason: %d", err);
		}
		mRenderTargetUID[i] = uid;
	}
	glDrawBuffers(numDrawBuffers, drawBuffers);

	// Check for any GL errors
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not apply render targets. Reason: %d", err);

	// Check if all worked fine and unbind the FBO
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch (status)
	{
	case GL_FRAMEBUFFER_COMPLETE_EXT:
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
		THROW_EXCEPTION(RenderingException, "Framebuffer incomplete: Attachment is NOT complete.");
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
		THROW_EXCEPTION(RenderingException, "Framebuffer incomplete: No image is attached to FBO.");
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
		THROW_EXCEPTION(RenderingException, "Framebuffer incomplete: Attached images have different dimensions");
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
		THROW_EXCEPTION(RenderingException, "Framebuffer incomplete: Color attached images have different internal formats");
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
		THROW_EXCEPTION(RenderingException, "Framebuffer incomplete: Draw buffer");
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
		THROW_EXCEPTION(RenderingException, "Framebuffer incomplete: Read buffer");
		break;

	case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
		THROW_EXCEPTION(RenderingException, "Unsupported by FBO implementation");
		break;

	default:
		THROW_EXCEPTION(RenderingException, "Unknow error");
		break;
	}

	SetViewport(Rect(0, 0, size.x, size.y));
}

void RenderState::ApplyBuffers(const VertexBuffer* buffer, const IndexBuffer* indexBuffer)
{
	//
	// Update the render targets if they have been changed since the last draw
	//

	if (!mEffectApplied) {
		mEffectState->ApplyUniforms();
		mEffectApplied = true;
	}

	if (mApplyRenderTarget) {
		mApplyRenderTarget = false;
		ApplyRenderTargets();
	}

	BindVertexBuffer(buffer);
	BindIndexBuffer(indexBuffer);
}

EffectState* RenderState::GetEffectState(const Effect* effect)
{
	EffectStates::iterator it = mEffectStates.find(effect->GetUID());
	if (it == mEffectStates.end()) {
		EffectState* state = new EffectState(effect, this);
		mEffectStates.insert(std::make_pair(effect->GetUID(), std::shared_ptr<EffectState>(state)));
		return state;
	}

	return it->second.get();
}

GLenum RenderState::GetDepthFuncAsEnum(DepthFunc::Enum depthFunc)
{
	static const GLenum enums[DepthFunc::SIZE] = {
		GL_NEVER,
		GL_LESS,
		GL_EQUAL,
		GL_LEQUAL,
		GL_GREATER,
		GL_NOTEQUAL,
		GL_GEQUAL,
		GL_ALWAYS
	};

	return enums[(int)depthFunc];
}

GLenum RenderState::GetSrcFactorAsEnum(SrcFactor::Enum sfactor)
{
	static const GLenum enums[SrcFactor::SIZE] = {
		GL_ZERO,
		GL_ONE,
		GL_SRC_COLOR,
		GL_ONE_MINUS_SRC_COLOR,
		GL_SRC_ALPHA,
		GL_ONE_MINUS_SRC_ALPHA,
		GL_DST_ALPHA,
		GL_ONE_MINUS_DST_ALPHA,
		GL_DST_COLOR,
		GL_ONE_MINUS_DST_COLOR,
		GL_CONSTANT_COLOR,
		GL_ONE_MINUS_CONSTANT_COLOR,
		GL_CONSTANT_ALPHA,
		GL_ONE_MINUS_CONSTANT_ALPHA
	};

	return enums[(int)sfactor];
}

GLenum RenderState::GetDestFactorAsEnum(DestFactor::Enum dfactor)
{
	static const GLenum enums[DestFactor::SIZE] = {
		GL_ZERO,
		GL_ONE,
		GL_SRC_COLOR,
		GL_ONE_MINUS_SRC_COLOR,
		GL_SRC_ALPHA,
		GL_ONE_MINUS_SRC_ALPHA,
		GL_DST_ALPHA,
		GL_ONE_MINUS_DST_ALPHA,
		GL_DST_COLOR,
		GL_ONE_MINUS_DST_COLOR,
		GL_CONSTANT_COLOR,
		GL_ONE_MINUS_CONSTANT_COLOR,
		GL_CONSTANT_ALPHA,
		GL_ONE_MINUS_CONSTANT_ALPHA
	};

	return enums[(int)dfactor];
}

GLenum RenderState::GetCullFaceAsEnum(CullFace::Enum cullFace)
{
	static const GLenum enums[CullFace::SIZE] = {
		0,
		GL_CW,
		GL_CCW
	};

	return enums[(int)cullFace];
}

void RenderState::UnbindIndexBuffer()
{
	mIndexBufferUID = 0;
}

void RenderState::UnbindVertexBuffer()
{
	mVertexBufferUID = 0;
}

void RenderState::UnbindTexture()
{
	mTextureUID[mActiveTextureIndex] = mSamplerObjectUID[mActiveTextureIndex] = 0;
}

void RenderState::UnbindEffect()
{
	mEffectUID = 0;
}

bool RenderState::IsEffectActive(const Effect* effect) const
{
	assert_not_null(effect);
	const uint32 uid = effect->GetUID();
	return mEffectUID == uid;
}

uint32 RenderState::GetNextTextureIndex()
{
	const uint32 textureIndex = mNextTextureIndex++;
	if (mNextTextureIndex >= mMaxTextureIndexes)
		mNextTextureIndex = 0;
	return textureIndex;
}
