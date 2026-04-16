// ◦ Xyz ◦
#pragma once

#include  <functional>
#include <Object/FrameBuffer.h>

namespace Engine
{
	class DrawBuffer final {
	public:
		static void Draw(const FrameBuffer& buffer);
		static void Draw(unsigned int idTexture);
		static unsigned int QuadVAO();
	};
}
