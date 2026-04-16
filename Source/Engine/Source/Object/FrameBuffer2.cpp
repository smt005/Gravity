// ◦ Xyz ◦
#include "FrameBuffer2.h"
#include "glad/gl.h"
#include <Screen.h>

using namespace Engine;

void FrameBuffer2::Create()
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Engine::ScreenParams::Width(), Engine::ScreenParams::Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
}
void FrameBuffer2::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
}

unsigned int FrameBuffer2::GetTexture() const
{
	return tex;
}
