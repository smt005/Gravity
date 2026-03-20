// ◦ Xyz ◦
#pragma once

#include <GuiWindow/GuiWindow.h>
#include <string>

namespace Windows
{
	class GenerateWindow final : public Engine::GuiWindow
	{
	public:
		GenerateWindow(std::string_view name);
		~GenerateWindow() = default;
		void Render() override;
		void Update(double dTime) override;
		void OnResize() override;
		void OnOpen() override;
		
		void FixSize();

	public:
		inline static const std::string_view windowName = "GenerateWindow";
	};
}
