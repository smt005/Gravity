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
#include <Logs.h>
#include "../Spaces/Space.h"
#include "../Spaces/DefaultSpace.h"
#include "../Spaces/MainThreadSpace.h"
#include "../Spaces/MainThreadProtSpace.h"
#include "../Spaces/ParallelThreadSpace.h"
#include "../Spaces/MultiThreadSpace.h"

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

	static bool collapseBodies = SpaceManager::collapseBodies.load();
	if (ImGui::Checkbox(": collapse", &collapseBodies)) {
		SpaceManager::collapseBodies.store(collapseBodies);
	}

	ButtonRender<DefaultSpace>();
	ButtonRender<MainThreadProtSpace>();
	ButtonRender<MainThreadSpace>();
	ButtonRender<ParallelThreadSpace>();
	ButtonRender < MultiThreadSpace>();
}
