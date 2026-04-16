// ◦ Xyz ◦
#pragma once

#include  <functional>

namespace Engine
{
	class DrawBuffer final {
	public:
		static void InitPostDraw();
		static void PostDraw(std::function<void()> drawFun);
	};
}
