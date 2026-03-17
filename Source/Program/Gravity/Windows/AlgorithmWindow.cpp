// ◦ Xyz ◦

#include "AlgorithmWindow.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <GuiWindow/GuiWindows.h>
#include <Screen.h>
#include <StringUtils.h>
#include <Common/Common.h>
#include <Files/Settings.h>
#include <GuiWindow/ImGuiHelp.h>
#include "../Spaces/SpaceManager.h"

#include <Log.h>

using namespace Windows;

AlgorithmWindow::AlgorithmWindow(std::string_view name)
	: Engine::GuiWindow(std::string(name))
{
	SetFlags(ImGuiWindowFlags_AlwaysAutoResize);
	SetTitle("Type algorithm");
	CloseButton(true);
}

void AlgorithmWindow::OnResize()
{
	FixSize();
}

void AlgorithmWindow::OnOpen()
{
	OnResize();
}

void AlgorithmWindow::FixSize()
{
	ImVec2 windowPos = ImGui::GetWindowPos();
	if (windowPos.x < 0 || windowPos.y < 0) {
		if (windowPos.x < 0) {
			windowPos.x = 0;
		}
		if (windowPos.y < 0) {
			windowPos.y = 0;
		}
		ImGui::SetWindowPos(windowPos);
	}
}

void AlgorithmWindow::OnClose()
{
	const ImVec2& windowPos = ImGui::GetWindowPos();
	const ImVec2& windowSize = ImGui::GetWindowSize();
	LOG("OnClose: {}: [{}, {}] [{}, {}]", windowName, windowPos.x, windowPos.y, windowSize.x, windowSize.y);
}

void AlgorithmWindow::Render() {
	const static ImVec2 buttonSize(200.f, 20.f);
	const ImColor currentColor(0.9f, 0.6f, 0.3f, 1.f);
	const ImColor defaultColor(0.5f, 0.6f, 0.7f, 1.f);

	std::string text = TO_STRING("Current: {}", SpaceManager::Current().GetName());
	ImGui::Text(text.c_str());
	ImGui::Separator();

	{
		const std::string_view name = "Space";
		ImGuiColorHandler color(ImGuiCol_Button, name == SpaceManager::Current().GetName() ? currentColor : defaultColor);

		if (ImGui::Button(name.data(), buttonSize)) {
			SpaceManager::SetCurrent<Space>();
		}
	}

	{
		const std::string_view name = "MainThreadSpace";
		ImGuiColorHandler color(ImGuiCol_Button, name == SpaceManager::Current().GetName() ? currentColor : defaultColor);

		if (ImGui::Button(name.data(), buttonSize)) {
			SpaceManager::SetCurrent<MainThreadSpace>();
		}
	}
}

void AlgorithmWindow::Update(double dTime) {

}

void AlgorithmWindow::SwitchVisibleWindow()
{
	if (!OpenWindow()) {
		CloseWindow();
	}
}

bool AlgorithmWindow::OpenWindow()
{
	if (!Engine::GuiWindows::ExistWindow(windowName)) {
		Engine::GuiWindows::MakeWindow<AlgorithmWindow>(windowName)->GetName();
		return true;
	}

	return false;
}

void AlgorithmWindow::CloseWindow()
{
	Engine::GuiWindows::CloseWindow(windowName);
}
