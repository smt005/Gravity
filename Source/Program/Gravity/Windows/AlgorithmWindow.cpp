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
	_currentSpace = SpaceManager::Current().GetId();
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
}

void AlgorithmWindow::Render() {
	size_t spaceHash = typeid(*SpaceManager::CurrentPtr().get()).hash_code();
	std::string text = TO_STRING("Current: {}", SpaceManager::Current().GetName());
	ImGui::Text(text.c_str());
	ImGui::Separator();

	ButtonRender<Space>();
	ButtonRender<OneThreadSpace>();
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
