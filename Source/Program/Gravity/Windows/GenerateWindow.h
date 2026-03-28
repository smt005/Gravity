// ◦ Xyz ◦
#pragma once

#include <GuiWindow/GuiWindow.h>
#include <string>

namespace Windows
{
	template <typename T>
	struct InputValue;

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

	private:
		mystd::shared_ptr<InputValue<int>> _countInput;
		mystd::shared_ptr<InputValue<int>> _sizeInput;
		mystd::shared_ptr<InputValue<float>> _minMassInput;
		mystd::shared_ptr<InputValue<float>> _maxMassInput;

	public:
		inline static const std::string_view windowName = "GenerateWindow";
	};
}
