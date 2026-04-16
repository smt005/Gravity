// ◦ Xyz ◦
#pragma once

#include  <functional>
#include <Object/FrameBuffer2.h>

namespace Engine
{
	class DrawBuffer final {
	public:
		static void InitPostDraw();
		static void PostDraw(std::function<void()> drawFun);
		static void PostDraw(unsigned int idTexture, FrameBuffer2& buffermA, FrameBuffer2& bufferB);
		static void Draw(const FrameBuffer2& buffer);
		static void Draw(unsigned int idTexture);
		static unsigned int QuadVAO();
	};
}
