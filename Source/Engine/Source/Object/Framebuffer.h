// ◦ Xyz ◦
#pragma once

class Framebuffer final
{
public:
	Framebuffer();
	Framebuffer(unsigned int width, unsigned int height);
	~Framebuffer();

	Framebuffer(const Framebuffer&) = delete;
	Framebuffer(Framebuffer&&) = delete;
	Framebuffer& operator = (const Framebuffer&) = delete;
	Framebuffer& operator = (Framebuffer&&) = delete;

	unsigned int TextureId() const;
	void PushRender() const;
	void PopRender() const;

	void Init(unsigned int _width, unsigned int _height);
	void Init();

private:
	unsigned int _fbo;
	unsigned int _textureId;

	// TODO:
public:
	unsigned int width = 0;
	unsigned int height = 0;
};
