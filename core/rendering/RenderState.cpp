#include "../Memory.h"
#include "RenderState.h"
#include "RenderTarget2D.h"
#include "SamplerObject.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "effect/Effect.h"
#include "exception/RenderingException.h"
#include "../window/ActiveWindow.h"
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
/*mDepthRenderTarget(nullptr), mDepthRenderTargetType(GL_DEPTH_ATTACHMENT), mDepthRenderTargetUID(0),*/ mFrameBufferID(0), mApplyRenderTarget(false), mFrameBufferApplied(false),
mApplyEffectState(true), mMaxDrawBuffers(0), mMaxActiveTextures(0), mNextTextureIndex(0)
{
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
		THROW_EXCEPTION(RenderingException, "Coult not initialize this thread's rendering states. Reason: %d", error);

	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, (GLint*)&mMaxActiveTextures);
	mTextureUID.resize(mMaxActiveTextures, 0);
	mTextureTarget.resize(mMaxActiveTextures, 0);
	mSamplerObjectUID.resize(mMaxActiveTextures, 0);

	glGetIntegerv(GL_MAX_DRAW_BUFFERS, (GLint*)&mMaxDrawBuffers);
	mRenderTargetInfo.resize(mMaxDrawBuffers, RenderTargetInfo());
	for (uint32 i = 0; i < mMaxDrawBuffers; ++i) {
		mRenderTargetInfo[i].attachmentType = GL_COLOR_ATTACHMENT0 + i;
	}
	mDepthRenderTargetInfo.attachmentType = GL_DEPTH_ATTACHMENT;

	//
	// Create the vertex array used to draw the upcomming triangles onto the screen
	//

	glGenVertexArrays(1, &mVertexArrayID);
	glBindVertexArray(mVertexArrayID);

	error = glGetError();
	if (mVertexArrayID == 0 || error != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not create or bind the vertex array for this thread's rendering state. Reason: %d", error);

	glGenFramebuffers(1, &mFrameBufferID);
	error = glGetError();
	if (mFrameBufferID == 0 || error != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not create frame buffer object for this thread's rendering state. Reason: %d", error);
}

RenderState::~RenderState()
{
	if (mFrameBufferID != 0) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &mFrameBufferID);
		mFrameBufferID = 0;
	}

	if (mVertexArrayID != 0) {
		glBindVertexArray(0);
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

	if (clear != 0)
		glClear(clear);

#if defined(_DEBUG) || defined(RENDERING_TROUBLESHOOTING)
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not clear the screen");
#endif
}

EffectState* RenderState::ApplyEffect(const Effect* effect)
{
	assert_not_null(effect);

	mApplyRenderTarget = true;
	InvalidateRenderTargets();

	BindEffect(effect);

	SetDepthTest(effect->GetDepthTest());
	SetDepthFunc(effect->GetDepthFunc());
	SetBlend(effect->GetBlend());

	auto blendFunc = effect->GetBlendFunc();
	SetBlendFunc(blendFunc.sfactor, blendFunc.dfactor);

	SetCullFace(effect->GetCullFace());
	SetClearColor(effect->GetClearColor());
	SetClearDepth(effect->GetClearDepth());

	return mEffectState;
}

void RenderState::BindEffect(const Effect* effect)
{
	mApplyEffectState = true;
	const uint32 uid = effect->GetUID();
	if (uid != mEffectUID) {
		glUseProgram(effect->GetProgramID());
		mEffectState = GetEffectState(effect);
		mEffectUID = uid;

#if defined(_DEBUG) || defined(RENDERING_TROUBLESHOOTING)
		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
			THROW_EXCEPTION(RenderingException, "Could not change which program to use whenever render vertices on the screen");
#endif
	}
}

void RenderState::InvalidateRenderTargets()
{
	for (uint32 i = 0; i < mMaxDrawBuffers; ++i) {
		RenderTargetInfo& info = mRenderTargetInfo[i];
		if (info.texture != nullptr) {
			info.texture = nullptr;
			info.uid = 0;
			info.dirty = true;
		}
	}

	if (mDepthRenderTargetInfo.texture != nullptr) {
		mDepthRenderTargetInfo.dirty = true;
		mDepthRenderTargetInfo.texture = nullptr;
		mDepthRenderTargetInfo.uid = 0;
	}
}

void RenderState::Render(const VertexBuffer* buffer)
{
	assert_not_null(buffer);
	Render(buffer, nullptr, 0);
}

void RenderState::Render(const VertexBuffer* buffer, const IndexBuffer* indexBuffer)
{
	assert_not_null(buffer);
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

#if defined(_DEBUG) || defined(RENDERING_TROUBLESHOOTING)
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not render index and vertex buffers");
#endif
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

#if defined(_DEBUG) || defined(RENDERING_TROUBLESHOOTING)
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not render index and vertex buffers");
#endif
}

void RenderState::BindVertexBuffer(const VertexBuffer* vertexBuffer)
{
	assert_not_null(vertexBuffer);
	const uint32 uid = vertexBuffer->GetUID();
	if (mVertexBufferUID == uid)
		return;

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->GetBufferID());

#if defined(_DEBUG) || defined(RENDERING_TROUBLESHOOTING)
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not bind the supplied vertex buffer");
#endif

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

	//
	// Set the vertex attributes
	//
	
	bool boundAttribLocations[MAX_VERTEX_ELEMENT_DESC];
	uint32 offset = 0;
	for (uint32 i = 0; i < 8; ++i) {
		const VertexElementDesc& elemDesc = desc.elements[i];
		if (elemDesc.fieldSize == 0) {
			break;
		}

		glEnableVertexAttribArray(elemDesc.location);
		boundAttribLocations[elemDesc.location] = true;
		const VertexElementType::Enum type = elemDesc.type;
		switch (type) {
		case VertexElementType::BYTE:
		case VertexElementType::UNSIGNED_BYTE:
		case VertexElementType::SHORT:
		case VertexElementType::UNSIGNED_SHORT:
		case VertexElementType::INT:
		case VertexElementType::UNSIGNED_INT:
			glVertexAttribIPointer(elemDesc.location, 
				elemDesc.numElementsInField,
				VertexElementType::Parse(elemDesc.type), 
				stride, 
				OFFSET(offset));
			break;
		case VertexElementType::FLOAT:
			glVertexAttribPointer(elemDesc.location, 
				elemDesc.numElementsInField,
				VertexElementType::Parse(elemDesc.type), 
				elemDesc.normalize ? GL_TRUE : GL_FALSE,
				stride, 
				OFFSET(offset));
			break;
		case VertexElementType::DOUBLE:
			glVertexAttribLPointer(elemDesc.location,
				elemDesc.numElementsInField,
				VertexElementType::Parse(elemDesc.type),
				stride,
				OFFSET(offset));
			break;
		}

		offset += elemDesc.fieldSize;
	}

#if defined(_DEBUG) || defined(RENDERING_TROUBLESHOOTING)
	err = glGetError();
	if (err != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not set vertex attrib location for the vertex array object");
#endif

	//
	// Unbind unused attrib locations
	//

	for (uint32 i = 0; i < MAX_VERTEX_ELEMENT_DESC; ++i) {
		if (!boundAttribLocations[i])
			glDisableVertexAttribArray(i);
	}

	//
	// Unbind the vertex buffer when VAO is setup
	//

	glBindBuffer(GL_ARRAY_BUFFER, 0);

#if defined(_DEBUG) || defined(RENDERING_TROUBLESHOOTING)
	err = glGetError();
	if (err != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not disable vertex attrib locations for the vertex array object");
#endif

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

#if defined(_DEBUG) || defined(RENDERING_TROUBLESHOOTING)
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not bind the supplied index buffer");
#endif
}

void RenderState::SetViewport(const Rect& viewport)
{
	if (mViewport != viewport) {
		glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
		mViewport = viewport;
	}

#if defined(_DEBUG) || defined(RENDERING_TROUBLESHOOTING)
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not set the screen viewport");
#endif
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

#if defined(_DEBUG) || defined(RENDERING_TROUBLESHOOTING)
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not enable/disable depth testing");
#endif
}

void RenderState::SetDepthFunc(DepthFunc::Enum func)
{
	if (mDepthFunc == func)
		return;

	glDepthFunc(GetDepthFuncAsEnum(func));
	mDepthFunc = func;

#if defined(_DEBUG) || defined(RENDERING_TROUBLESHOOTING)
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not set the depth function used when render faces on the screen");
#endif
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

#if defined(_DEBUG) || defined(RENDERING_TROUBLESHOOTING)
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not enable/disable blending");
#endif
}

void RenderState::SetBlendFunc(SrcFactor::Enum sfactor, DestFactor::Enum dfactor)
{
	if (mBlendFunc.sfactor == sfactor && mBlendFunc.dfactor == dfactor)
		return;

	glBlendFunc(GetSrcFactorAsEnum(sfactor), GetDestFactorAsEnum(dfactor));
	mBlendFunc.sfactor = sfactor;
	mBlendFunc.dfactor = dfactor;

#if defined(_DEBUG) || defined(RENDERING_TROUBLESHOOTING)
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not change the blend functions the cull faces used when render faces on the screen");
#endif
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

#if defined(_DEBUG) || defined(RENDERING_TROUBLESHOOTING)
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not change the cull faces used when render faces on the screen");
#endif
}

void RenderState::SetClearColor(const Color& color)
{
	if (mClearColor == color)
		return;

	glClearColor(color.r, color.g, color.b, color.a);
	mClearColor = color;

#if defined(_DEBUG) || defined(RENDERING_TROUBLESHOOTING)
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not set the colors used whenever clearing the screen");
#endif
}

void RenderState::SetClearDepth(float32 depth)
{
	if (FLOAT_EQUALS(depth, mClearDepth))
		return;

	glClearDepth(depth);
	mClearDepth = depth;

#if defined(_DEBUG) || defined(RENDERING_TROUBLESHOOTING)
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not change the clearing depth");
#endif
}

void RenderState::BindTexture(const Texture* texture, uint32 index)
{
	assert(index < mMaxActiveTextures && "You are not allowed to bind that many textures");
	if (mActiveTextureIndex != index) {
		glActiveTexture(GL_TEXTURE0 + index);
		mActiveTextureIndex = index;

#if defined(_DEBUG) || defined(RENDERING_TROUBLESHOOTING)
		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
			THROW_EXCEPTION(RenderingException, "Could not activate texture index: %d", index);
#endif
	}

	const uint32 uid = texture != nullptr ? texture->GetUID() : 0;
	if (mTextureUID[index] == uid)
		return;

	const GLuint textureID = texture != nullptr ? texture->GetTextureID() : 0;
	const GLenum textureTarget = texture != nullptr ? texture->GetTextureTarget() : mTextureTarget[index];
	glBindTexture(textureTarget, textureID);

	mTextureUID[index] = uid;
	mTextureTarget[index] = textureTarget;

#if defined(_DEBUG) || defined(RENDERING_TROUBLESHOOTING)
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not bind texture");
#endif
}

void RenderState::BindSampler(SamplerObject* samplerObject, uint32 index)
{
	const uint32 uid = samplerObject != nullptr ? samplerObject->GetUID() : 0;
	if (mSamplerObjectUID[index] == uid)
		return;

	const GLuint samplerID = samplerObject != nullptr ? samplerObject->GetSamplerID() : 0;
	glBindSampler(index, samplerID);
	mSamplerObjectUID[index] = uid;

#if defined(_DEBUG) || defined(RENDERING_TROUBLESHOOTING)
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not bind the texture sampler object");
#endif
}

void RenderState::SetRenderTarget(const RenderTarget2D* renderTarget, uint32 index)
{
	assert(index < mMaxDrawBuffers && "You are not allowed to bind that many render targets");
	const uint32 uid = renderTarget != nullptr ? renderTarget->GetUID() : 0;
	RenderTargetInfo& info = mRenderTargetInfo[index];
	if (info.uid == uid)
		return;

	info.dirty = true;
	info.texture = renderTarget;
	if (renderTarget != nullptr) 
		info.textureTarget = renderTarget->GetTextureTarget();
	info.uid = uid;

	mApplyRenderTarget = true;
}

void RenderState::SetRenderTarget(const RenderTargetCube* renderTarget, TextureCubeSide::Enum side, uint32 index)
{
	assert(index < mMaxDrawBuffers && "You are not allowed to bind that many render targets");
	const uint32 uid = renderTarget != nullptr ? renderTarget->GetUID() : 0;
	GLenum textureTarget = TextureCubeSide::Parse(side);
	RenderTargetInfo& info = mRenderTargetInfo[index];
	if (info.uid == uid && info.textureTarget == textureTarget)
		return;

	info.dirty = true;
	info.texture = renderTarget;
	if (renderTarget != nullptr)
		info.textureTarget = textureTarget;
	info.uid = uid;

	mApplyRenderTarget = true;
}

void RenderState::SetRenderTarget(const RenderTargetCube* renderTarget, uint32 index)
{
	assert_not_implemented();
}

void RenderState::SetDepthRenderTarget(const RenderTarget2D* renderTarget)
{
	const uint32 uid = renderTarget != nullptr ? renderTarget->GetUID() : 0;
	if (mDepthRenderTargetInfo.uid == uid)
		return;

	mDepthRenderTargetInfo.dirty = true;
	mDepthRenderTargetInfo.texture = renderTarget;
	mDepthRenderTargetInfo.uid = uid;

	if (renderTarget != nullptr) {
		GLenum attachmentType = GL_DEPTH_ATTACHMENT;
		if (renderTarget->GetTextureFormat() == TextureFormat::DEPTH24_STENCIL8)
			attachmentType = GL_DEPTH_STENCIL_ATTACHMENT;

		mDepthRenderTargetInfo.attachmentType = attachmentType;
		mDepthRenderTargetInfo.textureTarget = renderTarget->GetTextureTarget();
	}

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
	if (mDepthRenderTargetInfo.texture != nullptr)
		return true;

	for (uint32 i = 0; i < mMaxDrawBuffers; ++i)
		if (mRenderTargetInfo[i].texture != nullptr)
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
			InvalidateRenderTargets();

#if defined(_DEBUG) || defined(RENDERING_TROUBLESHOOTING)
			GLenum err = glGetError();
			if (err != GL_NO_ERROR)
				THROW_EXCEPTION(RenderingException, "Could not unbind frame buffer object. Reason: %d", err);
#endif
		}

		const Size& windowSize = ActiveWindow::GetSize();
		Rect screenViewportSize = { 0, 0, windowSize.width, windowSize.height };
		SetViewport(screenViewportSize);
		return;
	}
	
	// Bind frame buffer if it's not already bound to this render context
	if (!mFrameBufferApplied) {
		glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);
		mFrameBufferApplied = true;

#if defined(_DEBUG) || defined(RENDERING_TROUBLESHOOTING)
		// Check for any GL errors
		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
			THROW_EXCEPTION(RenderingException, "Could not bind frame buffer");
#endif
	}
	
	Size size;
	if (mDepthRenderTargetInfo.dirty) {
		mDepthRenderTargetInfo.dirty = false;
		if (mDepthRenderTargetInfo.texture != nullptr) {
			glFramebufferTexture2D(GL_FRAMEBUFFER, 
				mDepthRenderTargetInfo.attachmentType, 
				mDepthRenderTargetInfo.textureTarget, 
				mDepthRenderTargetInfo.texture->GetTextureID(), 
				0);

#if defined(_DEBUG) || defined(RENDERING_TROUBLESHOOTING)
			GLenum err = glGetError();
			if (err != GL_NO_ERROR)
				THROW_EXCEPTION(RenderingException, "Could not attach depth render target to frame buffer. Reason: %d", err);
#endif
		}
		else {
			glFramebufferTexture2D(GL_FRAMEBUFFER, 
				mDepthRenderTargetInfo.attachmentType,
				mDepthRenderTargetInfo.textureTarget, 
				0, 
				0);

#if defined(_DEBUG) || defined(RENDERING_TROUBLESHOOTING)
			GLenum err = glGetError();
			if (err != GL_NO_ERROR)
				THROW_EXCEPTION(RenderingException, "Could not detach depth render target from the frame buffer. Reason: %d", err);
#endif
		}
	}

	std::vector<GLenum> drawBuffers(mMaxDrawBuffers, 0);
	uint32 numDrawBuffers = 0;
	for (uint32 i = 0; i < mMaxDrawBuffers; ++i) {
		RenderTargetInfo& rti = mRenderTargetInfo[i];
		if (rti.dirty) {
			rti.dirty = false;
			if (rti.texture != nullptr) {
				size = rti.texture->GetSize();
				drawBuffers[numDrawBuffers++] = rti.attachmentType;
				glFramebufferTexture2D(GL_FRAMEBUFFER, 
					rti.attachmentType, 
					rti.textureTarget, 
					rti.texture->GetTextureID(), 
					0);

#if defined(_DEBUG) || defined(RENDERING_TROUBLESHOOTING)
				GLenum err = glGetError();
				if (err != GL_NO_ERROR)
					THROW_EXCEPTION(RenderingException, "Could not attach render target to frame buffer. Reason: %d", err);
#endif
			}
			else {
				glFramebufferTexture2D(GL_FRAMEBUFFER, 
					rti.attachmentType, 
					rti.textureTarget, 
					0, 
					0);

#if defined(_DEBUG) || defined(RENDERING_TROUBLESHOOTING)
				GLenum err = glGetError();
				if (err != GL_NO_ERROR)
					THROW_EXCEPTION(RenderingException, "Could not detach render target from the frame buffer. Reason: %d", err);
#endif
			}
		}
		else {
			if (rti.texture != nullptr) {
				size = rti.texture->GetSize();
				drawBuffers[numDrawBuffers++] = rti.attachmentType;
			}
		}
	}
	glDrawBuffers(numDrawBuffers, &drawBuffers[0]);

#if defined(_DEBUG) || defined(RENDERING_TROUBLESHOOTING)
	// Check for any GL errors
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not apply render targets. Reason: %d", err);
#endif

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

	if (mApplyEffectState) {
		mApplyEffectState = false;
		mEffectState->ApplyUniforms();
	}

	if (mApplyRenderTarget) {
		mApplyRenderTarget = false;
		ApplyRenderTargets();
	}

	BindIndexBuffer(indexBuffer);
	BindVertexBuffer(buffer);
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
	if (mNextTextureIndex >= mMaxActiveTextures)
		mNextTextureIndex = 0;
	return textureIndex;
}
