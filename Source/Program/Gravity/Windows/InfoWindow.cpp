// ◦ Xyz ◦

#include "InfoWindow.h"
#include <GuiWindow/GuiWindows.h>
#include <imgui.h>
#include <Screen.h>
#include <StringUtils.h>
#include <Logs.h>

using namespace Windows;

InfoWindow::InfoWindow(std::string_view name, std::function<void()> content)
	: Engine::GuiWindow(std::string(name))
	, _content(content)
{
	SetFlags(ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);
}

void InfoWindow::Render()
{
	if (_content) {
		_content();
	}
}

void InfoWindow::OnOpen()
{
	using namespace Engine;

	const auto size = ImGui::GetWindowSize();
	const ImVec2 pos = { ScreenParams::Width() / 2.f - size.x, ScreenParams::Height() / 2.f - size.y };
	ImGui::SetWindowPos(pos);
}

void InfoWindow::ShowMessageWindow(std::list<InfoWindow::Element>&& elementArgs)
{
	const std::string nameWindow = TO_STRING("InfoWindow:{}", std::rand() % 1000);

	auto content = [nameWindow, elements = std::forward<std::list<InfoWindow::Element>>(elementArgs)]() {
		constexpr ImVec2 buttonSize(200.f, 28.f);

		for (const auto& [type, text, fun] : elements) {
			switch (type) {
			case TypeElement::TEXT: { ImGui::Text("%s", text.c_str()); } break;
			case TypeElement::BUTTON: {
				if (ImGui::Button(text.c_str(), buttonSize)) {
					if (fun) {
						fun();
					}
				}
			} break;
			case TypeElement::CLOSE_BUTTON: {
				const std::string textButton = !text.empty() ? text : "Close";
				if (ImGui::Button(textButton.data(), buttonSize)) {
					if (fun) {
						fun();
					}
					Engine::GuiWindows::CloseWindow(nameWindow);
				}
			} break;
			default: {};
			}
		}
	};

	
	auto window = Engine::GuiWindows::MakeWindow<InfoWindow>(nameWindow, std::move(content));
	window->SetTitle("Information");
}

