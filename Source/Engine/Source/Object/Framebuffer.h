// ◦ Xyz ◦
#pragma once

namespace Engine
{
	class FrameBuffer final
	{
	public:
		FrameBuffer() = default;
		~FrameBuffer();
		FrameBuffer(const FrameBuffer&) = delete;
		void operator = (const FrameBuffer&) = delete;
		FrameBuffer(FrameBuffer&& buffer);
		FrameBuffer& operator = (FrameBuffer&& buffer);

		void Create();
		void Clear();
		void Bind() const;
		void UnBind() const;		
		unsigned int GetTexture() const;
		
	private:
		unsigned int _fbo = 0;
		unsigned int _tex = 0;
	};
}
