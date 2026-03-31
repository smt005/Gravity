// ◦ Xyz ◦
#pragma once

#include <GuiWindow/GuiWindow.h>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <unordered_map>

namespace Windows
{
	class RenderWindow final : public Engine::GuiWindow
	{
	public:
		RenderWindow(std::string_view name);
		~RenderWindow() = default;
		void Render() override;

	public:
		inline static const std::string_view windowName = "RenderWindow";
	};
}
