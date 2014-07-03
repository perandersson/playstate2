#include "../Memory.h"
#include "OpenGLRenderContext.h"
#include "Texture2D.h"
#include "RenderTarget2D.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RenderState.h"
#include "exception/RenderingException.h"
#include "effect/Effect.h"
using namespace core;

OpenGLRenderContext::OpenGLRenderContext()
: mGLEWContext(nullptr)
{
	mGLEWContext = new GLEWContext;
	memset(mGLEWContext, 0, sizeof(GLEWContext));
}

OpenGLRenderContext::~OpenGLRenderContext()
{
	mRenderStatesCreatedByContext.clear();

	if (mGLEWContext != nullptr) {
		delete mGLEWContext;
		mGLEWContext = nullptr;
	}
}

RenderState* OpenGLRenderContext::GetRenderState()
{
	// Retrieves the render state bound to the current thread. If no one exists then create a new one
	RenderState* renderState = RenderState::GetThreadLocal();
	if (renderState == nullptr) {
		renderState = new RenderState();
		// Put the instance inside this render context (making it the owner of the render state)
		// This is to ensure that they are removed correctly
		mRenderStatesCreatedByContext.push_back(std::shared_ptr<RenderState>(renderState));
		RenderState::SetThreadLocal(renderState);
	}

	return renderState;
}

RenderState* OpenGLRenderContext::Activate(Effect* effect)
{
	assert_not_null(effect);
	RenderState* state = GetRenderState();
	state->BindEffect(effect);
	return state;
}

RenderState* OpenGLRenderContext::Activate(Resource<Effect> effect)
{
	return Activate(effect.Get());
}

IndexBuffer* OpenGLRenderContext::CreateStaticBuffer(const uint32* indices, uint32 numIndices)
{
	assert_not_null(indices);
	assert(numIndices > 0 && "There is no point in creating a static buffer with 0 elements");


	const GLuint bufferID = GenBufferID();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(uint32), indices, GL_STATIC_DRAW);
	glFlush();

	GetRenderState()->UnbindIndexBuffer();

	GLenum status = glGetError();
	if (status != GL_NO_ERROR) {
		THROW_EXCEPTION(RenderingException, "Could not create vertex buffer. Reason: %d", status);
	}

	return new IndexBuffer(bufferID, numIndices);
}

VertexBuffer* OpenGLRenderContext::CreateStaticBuffer(const PositionTextureVertexType* vertices, uint32 numVertices)
{
	return CreateStaticBuffer(vertices, sizeof(PositionTextureVertexType), PositionTextureVertexTypeDeclaration, numVertices);
}

VertexBuffer* OpenGLRenderContext::CreateStaticBuffer(const PositionTextureNormalVertexType* vertices, uint32 numVertices)
{
	return CreateStaticBuffer(vertices, sizeof(PositionTextureNormalVertexType), PositionTextureNormalVertexTypeDeclaration, numVertices);
}

VertexBuffer* OpenGLRenderContext::CreateStaticBuffer(const void* vertices, uint32 sizeOfOneVertex, const VertexDesc& vertexDesc, uint32 numVertices)
{
	assert_not_null(vertices);
	assert(sizeOfOneVertex > 0 && "There is no point in creating a static buffer without any data");
	assert(numVertices > 0 && "There is no point in creating a static buffer with 0 elements");

	const GLuint bufferID = GenBufferID();

	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeOfOneVertex, vertices, GL_STATIC_DRAW);
	glFlush();

	GetRenderState()->UnbindVertexBuffer();

	GLenum status = glGetError();
	if (status != GL_NO_ERROR) {
		THROW_EXCEPTION(RenderingException, "Could not create vertex buffer. Reason: %d", status);
	}

	return new VertexBuffer(bufferID, vertexDesc, numVertices, sizeOfOneVertex);
}

RenderTarget2D* OpenGLRenderContext::CreateRenderTarget2D(const Size& size, TextureFormat::Enum format)
{
	assert(size.x > 0.0f && "You cannot create a render target with 0 width");
	assert(size.y > 0.0f && "You cannot create a render target with 0 height");

	GLenum _minMag = GL_LINEAR;
	const GLenum _format = GetTextureFormatAsEnum(format);
	const GLenum _internalFormat = GetInternalTextureFormatAsEnum(format);
	const GLenum minFilter = GetMinFilterAsEnum(MinFilter::DEFAULT);
	const GLenum magFilter = GetMagFilterAsEnum(MagFilter::DEFAULT);
	const GLenum textureWrap = GetTextureWrapAsEnum(TextureWrap::DEFAULT);

	switch (format)
	{
	case TextureFormat::DEPTH24:
		_minMag = GL_NEAREST;
		break;
	case TextureFormat::DEPTH24_STENCIL8:
		_minMag = GL_NEAREST;
		break;
	}

	const GLuint textureID = GenTextureID();
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, _internalFormat, size.x, size.y, 0, _format, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrap);
	glFlush();

	GetRenderState()->UnbindTexture();
	
	GLenum status = glGetError();
	if (status != GL_NO_ERROR) {
		THROW_EXCEPTION(RenderingException, "Could not create 2D render target. Reason: %d", status);
	}

	return new RenderTarget2D(textureID, size, format);
}

Texture2D* OpenGLRenderContext::CreateTexture2D(const Size& size, TextureFormat::Enum format, const byte* bytes)
{
	assert_not_null(bytes);
	assert(size.x > 0.0f && "You cannot create a texture with 0 width");
	assert(size.y > 0.0f && "You cannot create a texture with 0 height");

	const GLenum _format = GetTextureFormatAsEnum(format);
	const GLenum _internalFormat = GetInternalTextureFormatAsEnum(format);
	const GLenum minFilter = GetMinFilterAsEnum(MinFilter::DEFAULT);
	const GLenum magFilter = GetMagFilterAsEnum(MagFilter::DEFAULT);
	const GLenum textureWrap = GetTextureWrapAsEnum(TextureWrap::DEFAULT);

	const GLuint textureID = GenTextureID();
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, _internalFormat, size.width, size.height, 0, _format, GL_UNSIGNED_BYTE, bytes);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrap);
	glFlush();

	GetRenderState()->UnbindTexture();

	GLenum status = glGetError();
	if (status != GL_NO_ERROR) {
		THROW_EXCEPTION(RenderingException, "Could not create 2D texture. Reason: %d", status);
	}

	return new Texture2D(textureID, size, format);
}

TextureCube* OpenGLRenderContext::CreateTextureCube(const Size& size, TextureFormat::Enum format, const byte* positiveX, const byte* negativeX,
	const byte* positiveY, const byte* negativeY, const byte* positiveZ, const byte* negativeZ)
{
	assert_not_null(positiveX);
	assert_not_null(negativeX);
	assert_not_null(positiveY);
	assert_not_null(negativeY);
	assert_not_null(positiveZ);
	assert_not_null(negativeZ);
	assert(size.x > 0.0f && "You cannot create a texture with 0 width");
	assert(size.y > 0.0f && "You cannot create a texture with 0 height");

	const GLenum _format = GetTextureFormatAsEnum(format);
	const GLenum _internalFormat = GetInternalTextureFormatAsEnum(format);
	const GLenum minFilter = GetMinFilterAsEnum(MinFilter::DEFAULT);
	const GLenum magFilter = GetMagFilterAsEnum(MagFilter::DEFAULT);
	const GLenum textureWrap = GetTextureWrapAsEnum(TextureWrap::DEFAULT);

	const GLuint textureID = GenTextureID();
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	const byte* bytes[6] = {
		positiveX,
		negativeX,
		positiveY,
		negativeY,
		positiveZ,
		negativeZ
	};

	const GLenum types[6] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	for (uint32 i = 0; i < 6; ++i) {
		glTexImage2D(types[i], 0, _internalFormat, size.width, size.height, 0, _format, GL_UNSIGNED_BYTE, bytes[i]);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrap);

	GetRenderState()->UnbindTexture();

	GLenum status = glGetError();
	if (status != GL_NO_ERROR) {
		THROW_EXCEPTION(RenderingException, "Could not create 2D texture. Reason: %d", status);
	}

	return new TextureCube(textureID, size, format);
}

GLenum OpenGLRenderContext::GetTextureFormatAsEnum(TextureFormat::Enum format)
{
	GLint _format = GL_RGBA;
	switch (format)
	{
	case TextureFormat::RGB:
		_format = GL_RGB;
		break;
	case TextureFormat::RGBA:
		break;
	case TextureFormat::RGBA8:
		_format = GL_RGBA;
		break;
	case TextureFormat::RGBA12:
		_format = GL_RGBA;
		break;
	case TextureFormat::RGBA16:
		_format = GL_RGBA;
		break;
	case TextureFormat::RGB10_A2:
		_format = GL_RGBA;
		break;
	case TextureFormat::RGBA16F:
		_format = GL_RGBA;
		break;
	case TextureFormat::DEPTH24:
		_format = GL_DEPTH_COMPONENT;
		break;
	case TextureFormat::DEPTH24_STENCIL8:
		_format = GL_DEPTH_STENCIL;
		break;
	case TextureFormat::R:
		_format = GL_RED;
		break;
	case TextureFormat::BGR:
		_format = GL_BGR;
		break;
	case TextureFormat::BGRA:
		_format = GL_BGRA;
		break;
	}
	return _format;
}

GLenum OpenGLRenderContext::GetInternalTextureFormatAsEnum(TextureFormat::Enum format)
{
	GLint _internalFormat = GL_RGBA;
	switch (format)
	{
	case TextureFormat::RGB:
		_internalFormat = GL_RGB;
		break;
	case TextureFormat::RGBA:
		break;
	case TextureFormat::RGBA8:
		_internalFormat = GL_RGBA8;
		break;
	case TextureFormat::RGBA12:
		_internalFormat = GL_RGBA12;
		break;
	case TextureFormat::RGBA16:
		_internalFormat = GL_RGBA16;
		break;
	case TextureFormat::RGB10_A2:
		_internalFormat = GL_RGB10_A2;
		break;
	case TextureFormat::RGBA16F:
		_internalFormat = GL_RGBA16F;
		break;
	case TextureFormat::DEPTH24:
		_internalFormat = GL_DEPTH_COMPONENT24;
		break;
	case TextureFormat::DEPTH24_STENCIL8:
		_internalFormat = GL_DEPTH24_STENCIL8;
		break;
	case TextureFormat::R:
		_internalFormat = GL_R8;
		break;
	case TextureFormat::BGR:
		_internalFormat = GL_RGB;
		break;
	case TextureFormat::BGRA:
		_internalFormat = GL_RGBA;
		break;
	}
	return _internalFormat;
}

GLenum OpenGLRenderContext::GetMinFilterAsEnum(MinFilter::Enum minFilter)
{
	return MinFilter::Parse(minFilter);
}

GLenum OpenGLRenderContext::GetMagFilterAsEnum(MagFilter::Enum magFilter)
{
	return MagFilter::Parse(magFilter);
}

GLenum OpenGLRenderContext::GetTextureWrapAsEnum(TextureWrap::Enum textureWrap)
{
	return TextureWrap::Parse(textureWrap);
}

GLuint OpenGLRenderContext::GenTextureID() const
{
	GLuint id = 0;
	glGenTextures(1, &id);

	GLenum error = glGetError();
	if (id == 0 || error != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not generate texture ID");

	return id;
}

GLuint OpenGLRenderContext::GenBufferID() const
{
	GLuint id = 0;
	glGenBuffers(1, &id);

	GLenum error = glGetError();
	if (id == 0 || error != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not generate buffer ID");

	return id;
}
