// ◦ Xyz ◦
#pragma once

class Framebuffer final
{
public:
	Framebuffer(bool init = false);
	Framebuffer(unsigned int width, unsigned int height, bool init = false);
	~Framebuffer();

	Framebuffer(const Framebuffer&) = delete;
	Framebuffer(Framebuffer&&) = delete;
	Framebuffer& operator = (const Framebuffer&) = delete;
	Framebuffer& operator = (Framebuffer&&) = delete;

	unsigned int TextureId() const;
	void PushRender(bool clean = false) const;
	void PopRender() const;

	void Init(unsigned int _width, unsigned int _height);
	void Init();
	void Swap(Framebuffer& buffer);
	void Clear();

private:
	unsigned int _fbo;
	unsigned int _textureId;

	// TODO:
public:
	unsigned int width = 0;
	unsigned int height = 0;
};
