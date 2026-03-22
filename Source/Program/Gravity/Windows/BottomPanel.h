// ◦ Xyz ◦
#pragma once

#include <GuiWindow/GuiWindow.h>

namespace Windows
{
	class BottomPanel final : public Engine::GuiWindow
	{
	public:
		BottomPanel(std::string_view name);
		~BottomPanel() = default;
		void Render() override;
		void OnResize() override;
		void OnOpen() override;

	public:
		inline static const std::string_view windowName = "BottomPanel";
	};
}
