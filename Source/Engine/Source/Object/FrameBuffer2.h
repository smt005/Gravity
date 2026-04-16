// ◦ Xyz ◦
#pragma once

namespace Engine
{
	class FrameBuffer2
	{
	public:
		void Create();
		void Bind() const;
		unsigned int GetTexture() const;
		
	private:
		unsigned int fbo;
		unsigned int tex;
	};
}
