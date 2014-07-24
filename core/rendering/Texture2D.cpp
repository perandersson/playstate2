#include "../Memory.h"
#include "Texture2D.h"
#include "RenderContext.h"
#include "RenderState.h"
#include "OpenGLEnum.h"
#include "exception/RenderingException.h"
using namespace core;

Texture2D::Texture2D(GLuint textureID, const Size& size, TextureFormat::Enum format)
: Texture(textureID, GL_TEXTURE_2D, size, format)
{
}

Texture2D::~Texture2D()
{
}

void Texture2D::Resize(const Size& newSize)
{
	assert(newSize.x > 0.0f && "You cannot resize a texture to 0 width");
	assert(newSize.y > 0.0f && "You cannot resize a texture to 0 height");

	auto renderState = RenderContext::GetRenderState();

	//
	// Bind the texture
	//

	renderState->BindTexture(this, 0);

	//
	// Resize the texture
	//

	const TextureFormat::Enum format = GetTextureFormat();
	const GLenum _format = OpenGLEnum::ConvertToTextureFormatEnum(format);
	const GLenum _internalFormat = OpenGLEnum::ConvertToInternalTextureFormatEnum(format);

	Lock();
	glTexImage2D(this->GetTextureTarget(), 0, _internalFormat, newSize.width, newSize.height, 0, _format, GL_UNSIGNED_BYTE, NULL);
	glFlush();
	Unlock();

	mSize = newSize;

	//
	// Make sure that the texture is not bound anymore
	//

	renderState->UnbindTexture();

	GLenum status = glGetError();
	if (status != GL_NO_ERROR) {
		THROW_EXCEPTION(RenderingException, "Could not resize the supplied 2D texture. Reason: %d", status);
	}
}

void Texture2D::Update(Color* pixels)
{
	Update(mSize, pixels);
}

void Texture2D::Update(const Size& size, Color* pixels)
{
	assert_not_null(pixels);
	assert(size.width > 0 && "You update a texture with 0 height");
	assert(size.height > 0 && "You update a texture with 0 height");

	RenderState* state = RenderContext::GetRenderState();

	//
	// Bind the texture
	//

	state->BindTexture(this, 0);

	//
	// Update the texture pixels
	//

	const TextureFormat::Enum format = GetTextureFormat();
	const GLenum _format = OpenGLEnum::ConvertToTextureFormatEnum(format);
	const GLenum _internalFormat = OpenGLEnum::ConvertToInternalTextureFormatEnum(format);

	Lock();
	glTexImage2D(this->GetTextureTarget(), 0, _internalFormat, size.width, size.height, 0, _format, GL_FLOAT, pixels->_array);
	glFlush();
	Unlock();

	mSize = size;

	//
	// Make sure that the texture is not bound anymore
	//

	state->UnbindTexture();
	
	GLenum status = glGetError();
	if (status != GL_NO_ERROR) {
		THROW_EXCEPTION(RenderingException, "Could not create 2D texture. Reason: %d", status);
	}
}
