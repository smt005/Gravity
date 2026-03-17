// ◦ Xyz ◦
#pragma once

#include <GuiWindow/GuiWindow.h>
#include <string>
#include <functional>

namespace Windows
{
	class AlgorithmWindow final : public Engine::GuiWindow
	{
	public:
		AlgorithmWindow(std::string_view name);
		~AlgorithmWindow() = default;
		void Render() override;
		void Update(double dTime) override;
		void OnResize() override;
		void OnOpen() override;
		void OnClose() override;

		void FixSize();

	public:
		static void SwitchVisibleWindow();
		static bool OpenWindow();
		static void CloseWindow();

	public:
		inline static const std::string_view windowName = "AlgorithmWindow";
	};
}
