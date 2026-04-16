// ◦ Xyz ◦

#include "Framebuffer.h"
#include <exception>
#include "glad/gl.h"
#include <Screen.h>

Framebuffer::Framebuffer(bool init)
	: width(Engine::ScreenParams::Width())
	, height(Engine::ScreenParams::Height())
{
	if (init) {
		Init();
	}
}

Framebuffer::Framebuffer(unsigned int _width, unsigned int _height, bool init)
	: width(_width)
	, height(_height)
{
	if (init) {
		Init();
	}
}

void Framebuffer::Init(unsigned int _width, unsigned int _height)
{
	width = _width;
	height = _height;
	Init();
}

void Framebuffer::Init()
{
	this->~Framebuffer();

	// Создание фреймбуфера (FBO)
	glGenFramebuffers(1, &_fbo);
	//glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

	// Создание текстуры для хранения изображения
	glGenTextures(1, &_textureId);
	glBindTexture(GL_TEXTURE_2D, _textureId);

	// Устанавливаем параметры текстуры
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Создаём хранилище для текстуры (без данных)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	//glBindTexture(GL_TEXTURE_2D, 0); // Отвязываем текстуру

	// Привязка текстуры к фреймбуферу
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _textureId, 0);

	glBindTexture(GL_TEXTURE_2D, 0); // Отвязываем текстуру

	// Проверка готовности фреймбуфера
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		throw std::exception("Framebuffer is not complete: ");
	}
}

void Framebuffer::Swap(Framebuffer& buffer)
{
	std::swap(_fbo, buffer._fbo);
	std::swap(_textureId, buffer._textureId);
	std::swap(width, buffer.width);
	std::swap(height, buffer.height);
}

void Framebuffer::Clear()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::~Framebuffer()
{
	if (_fbo != 0) {
		glDeleteFramebuffers(1, &_fbo);
		_fbo = 0;
	}

	if (_textureId != 0) {
		glDeleteTextures(1, &_textureId);
		_textureId = 0;
	}
}

unsigned int Framebuffer::TextureId() const
{
	return _textureId;
}

void Framebuffer::PushRender(bool clean) const
{
	// Привязываем фреймбуфер для рендеринга
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	
	if (clean) {
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}

void Framebuffer::PopRender() const
{
	// Отвязываем фреймбуфер, возвращаясь к рендерингу на экран
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
