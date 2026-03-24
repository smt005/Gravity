// ◦ Xyz ◦
#pragma once

#include <GuiWindow/GuiWindow.h>
#include <string>
#include <functional>

namespace Windows
{
	class InfoWindow final : public Engine::GuiWindow
	{
	public:
		enum class TypeElement {
			TEXT,
			BUTTON,
			CLOSE_BUTTON
		};
		using Element = std::tuple<TypeElement, std::string, std::function<void()>>;

	public:
		InfoWindow(std::string_view name, std::function<void()> content);
		void Render() override;
		void OnOpen() override;

	private:
		std::function<void()> _content;

	public:
		static void ShowMessageWindow(std::list<InfoWindow::Element>&& elementArgs);
	};
}
