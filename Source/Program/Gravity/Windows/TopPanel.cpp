// ◦ Xyz ◦

#include "TopPanel.h"
#include <GuiWindow/GuiWindows.h>
#include <imgui.h>
#include <Screen.h>
#include <StringUtils.h>
#include <Log.h>

using namespace Windows;

TopPanel::TopPanel(std::string_view name)
	: Engine::GuiWindow(std::string(name))
{
	SetFlags(ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground);

}

void TopPanel::OnResize()
{
	const float width = static_cast<float>(Engine::ScreenParams::Width());
	const float height = static_cast<float>(Engine::ScreenParams::Height());

	ImGui::SetWindowPos(GetTitle().c_str(), { 0.f, 0.f });
	ImGui::SetWindowSize(GetTitle().c_str(), { width, 30.f });
}

void TopPanel::OnOpen()
{
	SetAlpha(0.f);
	ImGui::SetWindowPos(GetTitle().c_str(), { 0.f, 0.f });
	OnResize();
}

void TopPanel::Render() {
	{
		ImVec4 color = { 1.f, 1.f, 1.f, 1.f };
		if (_fps < 10) {
			color = { 1.f, 0.f, 0.f, 1.f };
		}
		if (_fps >= 30) {
			color = { 0.f, 1.f, 0.f, 1.f };
		}

		std::string_view prefix;
		#ifdef _DEBUG
			prefix = "DEBUG";
		#endif

		std::string text = TO_STRING("{} fps: {} ", prefix, _fps);
		ImGui::TextColored(color, text.c_str());
	}

	{
		std::string text = TO_STRING("[min: {} middle: {} max: {}]", _minFps, (_maxFps + _minFps) / 2, _maxFps);
		ImGui::SameLine();
		ImGui::Text(text.c_str());
	}

	ImGui::SameLine();
	if (ImGui::Button("Reset", { 40.f, 16.f })) {
		timeOut = 0.5;
		_fps = 0;
		_minFps = std::numeric_limits<int>::max();
		_maxFps = std::numeric_limits<int>::min();
	}
}

void TopPanel::Update(double dTime) {
	if (timeOut > 0 || dTime == 0) {
		timeOut -= dTime;
		return;
	}

	_fps = 1 / dTime;
	_minFps = std::min(_fps, _minFps);
	_maxFps = std::max(_fps, _maxFps);
}


void TopPanel::SwitchVisibleWindow()
{
	if (!OpenWindow()) {
		CloseWindow();
	}
}

bool TopPanel::OpenWindow()
{
	if (!Engine::GuiWindows::ExistWindow(windowName)) {
		Engine::GuiWindows::MakeWindow<TopPanel>(windowName)->GetName();
		return true;
	}

	return false;
}

void TopPanel::CloseWindow()
{
	Engine::GuiWindows::CloseWindow(windowName);
}
