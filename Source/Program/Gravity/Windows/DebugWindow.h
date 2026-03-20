// ◦ Xyz ◦
#pragma once

#include <GuiWindow/GuiWindow.h>
#include <string>

namespace Windows
{
	class DebugWindow final : public Engine::GuiWindow
	{
	public:
		DebugWindow(std::string_view name);
		~DebugWindow() = default;
		void Render() override;
		void Update(double dTime) override;
		void OnOpen() override;
		void OnClose() override;

		void CameraToMinMassObject();
		void CameraToMaxMassObject();

	private:
		int _fps = 0;
		int _minFps = std::numeric_limits<int>::max();
		int _maxFps = std::numeric_limits<int>::min();
		double timeOut = 1;

	public:
		inline static const std::string_view windowName = "DebugWindow";
	};
}
