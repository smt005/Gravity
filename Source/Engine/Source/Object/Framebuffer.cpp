// ◦ Xyz ◦

#include "FrameBuffer.h"
#include <exception>
#include "glad/gl.h"
#include <Screen.h>

using namespace Engine;

FrameBuffer::~FrameBuffer()
{
	if (_fbo != 0) {
		glDeleteFramebuffers(1, &_fbo);
		_fbo = 0;
	}

	if (_tex != 0) {
		glDeleteTextures(1, &_tex);
		_tex = 0;
	}
}

FrameBuffer::FrameBuffer(FrameBuffer&& buffer)
{
	std::swap(_fbo, buffer._fbo);
	std::swap(_tex, buffer._tex);
}

FrameBuffer& FrameBuffer::operator = (FrameBuffer&& buffer)
{
	std::swap(_fbo, buffer._fbo);
	std::swap(_tex, buffer._tex);
	return *this;
}

void FrameBuffer::Create()
{
	this->~FrameBuffer();

	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

	glGenTextures(1, &_tex);
	glBindTexture(GL_TEXTURE_2D, _tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Engine::ScreenParams::Width(), Engine::ScreenParams::Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _tex, 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		throw std::exception("[FrameBuffer::Create] Framebuffer is not complete: ");
	}
}

void FrameBuffer::Clear()
{
	this->~FrameBuffer();
}

void FrameBuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
}

void FrameBuffer::UnBind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int FrameBuffer::GetTexture() const
{
	return _tex;
}
