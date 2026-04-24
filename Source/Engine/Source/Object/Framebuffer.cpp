// ◦ Xyz ◦

#include "FrameBuffer.h"
#include <exception>
#include "glad/gl.h"
#include <FreeImage.h>
#include <Screen.h>
#include <Files/FileManager.h>

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

	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	UnBind();

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		throw std::exception("[FrameBuffer::Create] Framebuffer is not complete: ");
	}

	_width = Engine::ScreenParams::Width();
	_height = Engine::ScreenParams::Height();
}

void FrameBuffer::Clear() const
{
	Bind();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Bind(bool clearColor, bool clearDepth, float* color) const
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

	if (color) {
		glClearColor(color[0], color[1], color[2], color[3]);
	}

	if (clearColor && clearDepth) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else if (clearColor) {
		glClear(GL_COLOR_BUFFER_BIT);
	}
	else if (clearDepth) {
		glClear(GL_DEPTH_BUFFER_BIT);
	}
}

void FrameBuffer::UnBind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int FrameBuffer::GetTexture() const
{
	return _tex;
}

// TODO:
void FrameBuffer::Save(std::string_view fileNamePath) const
{
	if (_tex == 0 || _width == 0 || _height == 0) {
		return;
	}

	const std::filesystem::path fullFileName = Engine::FileManager::Get("write").GetRoot() / "Screenshots" / fileNamePath;

	// Определяем формат по расширению
	FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(fullFileName.string().c_str());
	if (fif == FIF_UNKNOWN) {
		fif = FIF_PNG; // fallback
	}

	// Будем сохранять в 32 бита (RGBA)
	const unsigned int bpp = 32;
	const unsigned int bytesPerPixel = bpp / 8;

	std::vector<BYTE> pixels(_width * _height * bytesPerPixel);

	glBindTexture(GL_TEXTURE_2D, _tex);

	// Читаем из GPU (обрати внимание — формат BGRA как при загрузке)
	glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels.data());

	// Создаём FreeImage bitmap
	FIBITMAP* bitmap = FreeImage_ConvertFromRawBits(
		pixels.data(),
		_width,
		_height,
		_width * bytesPerPixel,
		bpp,
		0xFF0000, // red mask
		0x00FF00, // green mask
		0x0000FF, // blue mask
		true      // top-down (важно!)
	);

	if (!bitmap) {
		return;
	}

	// Сохраняем
	if (!FreeImage_Save(fif, bitmap, fullFileName.string().c_str())) {
		FreeImage_Unload(bitmap);
		return;
	}

	FreeImage_Unload(bitmap);
}