#include "../Memory.h"
#include "OpenGLRenderContext.h"
#include "Texture2D.h"
#include "RenderTarget2D.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RenderState.h"
#include "OpenGLEnum.h"
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
	state->ApplyEffect(effect);
	return state;
}

RenderState* OpenGLRenderContext::Activate(Resource<Effect> effect)
{
	return Activate(effect.Get());
}

IndexBuffer* OpenGLRenderContext::CreateBuffer(const uint32* indices, uint32 numIndices, BufferUsage::Enum usage)
{
	assert_not_null(indices);
	assert(numIndices > 0 && "There is no point in creating a static buffer with 0 elements");


	const GLuint bufferID = GenBufferID();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(uint32), indices, OpenGLEnum::Convert(usage));
	glFlush();

	GetRenderState()->UnbindIndexBuffer();

	GLenum status = glGetError();
	if (status != GL_NO_ERROR) {
		THROW_EXCEPTION(RenderingException, "Could not create vertex buffer. Reason: %d", status);
	}

	return new IndexBuffer(bufferID, numIndices, usage);
}

VertexBuffer* OpenGLRenderContext::CreateBuffer(const PositionVertexType* vertices, PrimitiveType::Enum primitiveType, uint32 numVertices, BufferUsage::Enum usage)
{
	return CreateBuffer(vertices, sizeof(PositionVertexType), PositionVertexTypeDeclaration, primitiveType, numVertices, usage);
}

VertexBuffer* OpenGLRenderContext::CreateBuffer(const PositionTextureVertexType* vertices, PrimitiveType::Enum primitiveType, uint32 numVertices, BufferUsage::Enum usage)
{
	return CreateBuffer(vertices, sizeof(PositionTextureVertexType), PositionTextureVertexTypeDeclaration, primitiveType, numVertices, usage);
}

VertexBuffer* OpenGLRenderContext::CreateBuffer(const PositionTextureNormalVertexType* vertices, PrimitiveType::Enum primitiveType, uint32 numVertices, BufferUsage::Enum usage)
{
	return CreateBuffer(vertices, sizeof(PositionTextureNormalVertexType), PositionTextureNormalVertexTypeDeclaration, primitiveType, numVertices, usage);
}

VertexBuffer* OpenGLRenderContext::CreateBuffer(const void* vertices, uint32 sizeOfOneVertex, const VertexDesc& vertexDesc, PrimitiveType::Enum primitiveType, uint32 numVertices, BufferUsage::Enum usage)
{
	assert_not_null(vertices);
	assert(sizeOfOneVertex > 0 && "There is no point in creating a static buffer without any data");
	assert(numVertices > 0 && "There is no point in creating a static buffer with 0 elements");

	const GLuint bufferID = GenBufferID();

	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeOfOneVertex, vertices, OpenGLEnum::Convert(usage));
	glFlush();

	GetRenderState()->UnbindVertexBuffer();

	GLenum status = glGetError();
	if (status != GL_NO_ERROR) {
		THROW_EXCEPTION(RenderingException, "Could not create vertex buffer. Reason: %d", status);
	}

	return new VertexBuffer(bufferID, vertexDesc, primitiveType, numVertices, sizeOfOneVertex, usage);
}

RenderTarget2D* OpenGLRenderContext::CreateRenderTarget2D(const Size& size, TextureFormat::Enum format)
{
	assert(size.x > 0.0f && "You cannot create a render target with 0 width");
	assert(size.y > 0.0f && "You cannot create a render target with 0 height");

	GLenum _minMag = GL_LINEAR;
	const GLenum _format = OpenGLEnum::ConvertToTextureFormatEnum(format);
	const GLenum _internalFormat = OpenGLEnum::ConvertToInternalTextureFormatEnum(format);
	const GLenum minFilter = OpenGLEnum::Convert(MinFilter::DEFAULT);
	const GLenum magFilter = OpenGLEnum::Convert(MagFilter::DEFAULT);
	const GLenum textureWrap = OpenGLEnum::Convert(TextureWrap::DEFAULT);

	switch (format)
	{
	case TextureFormat::DEPTH24:
		_minMag = GL_NEAREST;
		break;
	case TextureFormat::DEPTH32F:
		_minMag = GL_NEAREST;
		break;
	case TextureFormat::DEPTH24_STENCIL8:
		_minMag = GL_NEAREST;
		break;
	case TextureFormat::DEPTH32F_STENCIL8:
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

	const GLenum _format = OpenGLEnum::ConvertToTextureFormatEnum(format);
	const GLenum _internalFormat = OpenGLEnum::ConvertToInternalTextureFormatEnum(format);
	const GLenum minFilter = OpenGLEnum::Convert(MinFilter::DEFAULT);
	const GLenum magFilter = OpenGLEnum::Convert(MagFilter::DEFAULT);
	const GLenum textureWrap = OpenGLEnum::Convert(TextureWrap::DEFAULT);

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

RenderTargetCube* OpenGLRenderContext::CreateRenderTargetCube(const Size& size, TextureFormat::Enum format)
{
	assert(size.x > 0.0f && "You cannot create a render target texture with 0 width");
	assert(size.y > 0.0f && "You cannot create a render target texture with 0 height");

	GLenum _minMag = GL_LINEAR;
	const GLenum _format = OpenGLEnum::ConvertToTextureFormatEnum(format);
	const GLenum _internalFormat = OpenGLEnum::ConvertToInternalTextureFormatEnum(format);
	const GLenum minFilter = OpenGLEnum::Convert(MinFilter::DEFAULT);
	const GLenum magFilter = OpenGLEnum::Convert(MagFilter::DEFAULT);
	const GLenum textureWrap = OpenGLEnum::Convert(TextureWrap::DEFAULT);

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
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	const GLenum types[6] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	for (uint32 i = 0; i < 6; ++i) {
		glTexImage2D(types[i], 0, _internalFormat, size.width, size.height, 0, _format, GL_FLOAT, NULL);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, textureWrap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, textureWrap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, textureWrap);

	GetRenderState()->UnbindTexture();

	glFlush();

	GLenum status = glGetError();
	if (status != GL_NO_ERROR) {
		THROW_EXCEPTION(RenderingException, "Could not create 2D render target. Reason: %d", status);
	}

	return new RenderTargetCube(textureID, size, format);
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

	const GLenum _format = OpenGLEnum::ConvertToTextureFormatEnum(format);
	const GLenum _internalFormat = OpenGLEnum::ConvertToInternalTextureFormatEnum(format);
	const GLenum minFilter = OpenGLEnum::Convert(MinFilter::DEFAULT);
	const GLenum magFilter = OpenGLEnum::Convert(MagFilter::DEFAULT);
	const GLenum textureWrap = OpenGLEnum::Convert(TextureWrap::DEFAULT);

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

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, textureWrap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, textureWrap);

	GetRenderState()->UnbindTexture();

	glFlush();

	GLenum status = glGetError();
	if (status != GL_NO_ERROR) {
		THROW_EXCEPTION(RenderingException, "Could not create 2D texture. Reason: %d", status);
	}

	return new TextureCube(textureID, size, format);
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
