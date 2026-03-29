// ◦ Xyz ◦
#pragma once

#include <GuiWindow/GuiWindow.h>
#include <string>

namespace Windows
{
	class TopPanel final : public Engine::GuiWindow
	{
	public:
		TopPanel(std::string_view name);
		~TopPanel() = default;
		void Render() override;
		void Update(double dTime) override;
		void OnResize() override;
		void OnOpen() override;

	private:
		double _fps = 0;
		double _ups = 0; // Update per second
		double _uTime = 0;

	public:
		inline static const std::string_view windowName = "TopPanel";
	};
}
