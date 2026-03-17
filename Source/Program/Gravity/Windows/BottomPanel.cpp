// ◦ Xyz ◦

#include "BottomPanel.h"
// ◦ Xyz ◦

#include "BottomPanel.h"
#include <GuiWindow/GuiWindows.h>
#include <imgui.h>
#include <Screen.h>
#include <StringUtils.h>
#include <Log.h>

using namespace Windows;

BottomPanel::BottomPanel(std::string_view name)
	: Engine::GuiWindow(std::string(name))
{
	SetFlags(ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground);

}

void BottomPanel::OnResize()
{
	const float widthWindow = static_cast<float>(Engine::ScreenParams::Width());
	const float heightWindow = static_cast<float>(Engine::ScreenParams::Height());

	constexpr float heightPanel = 50.f;
	constexpr float borderPanel = 5.f;
	ImGui::SetWindowPos(GetTitle().c_str(), { 0.f, heightWindow - (heightPanel + borderPanel) });
	ImGui::SetWindowSize(GetTitle().c_str(), { widthWindow, heightWindow });
}

void BottomPanel::OnOpen()
{
	SetAlpha(0.f);
	ImGui::SetWindowPos(GetTitle().c_str(), { 0.f, 0.f });
	OnResize();
}

void BottomPanel::Render() {

}

void BottomPanel::Update(double dTime) {

}

void BottomPanel::SwitchVisibleWindow()
{
	if (!OpenWindow()) {
		CloseWindow();
	}
}

bool BottomPanel::OpenWindow()
{
	if (!Engine::GuiWindows::ExistWindow(windowName)) {
		Engine::GuiWindows::MakeWindow<BottomPanel>(windowName)->GetName();
		return true;
	}

	return false;
}

void BottomPanel::CloseWindow()
{
	Engine::GuiWindows::CloseWindow(windowName);
}
