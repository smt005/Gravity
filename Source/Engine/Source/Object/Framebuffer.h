// ◦ Xyz ◦
#pragma once

#include <string>

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
		void Clear() const;
		void Bind(bool clearColor = true, bool clearDepth = true, float* color = nullptr) const;
		void UnBind() const;		
		unsigned int GetTexture() const;
		void Save(std::string_view fileNamePath) const;

	private:
		unsigned int _fbo = 0;
		unsigned int _tex = 0;
		int _width;
		int _height;
	};
}
