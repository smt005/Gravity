// ◦ Xyz ◦

#include "Framebuffer.h"
#include <exception>
#include "glad/gl.h"
#include <Screen.h>

Framebuffer::Framebuffer()
	: width(Engine::ScreenParams::Width())
	, height(Engine::ScreenParams::Height())
{}

Framebuffer::Framebuffer(unsigned int _width, unsigned int _height)
	: width(_width)
	, height(_height)
{}

void Framebuffer::Init(unsigned int _width, unsigned int _height)
{
	width = _width;
	height = _height;
	Init();
}

void Framebuffer::Init()
{
	// Создание фреймбуфера (FBO)
	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

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

Framebuffer::~Framebuffer()
{
	if (_fbo != 0) {
		glDeleteFramebuffers(1, &_fbo);
	}

	if (_textureId != 0) {
		glDeleteTextures(1, &_textureId);
	}
}

unsigned int Framebuffer::TextureId() const
{
	return _textureId;
}

void Framebuffer::PushRender() const
{
	// Привязываем фреймбуфер для рендеринга
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Framebuffer::PopRender() const
{
	// Отвязываем фреймбуфер, возвращаясь к рендерингу на экран
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
