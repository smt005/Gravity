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
#include "../Spaces/MainThreadSpaceOneBlock.h"
//#include "../Spaces/XXX.h"

using namespace Windows;

AlgorithmWindow::AlgorithmWindow(std::string_view name)
	: Engine::GuiWindow(std::string(name))
{
	SetFlags(ImGuiWindowFlags_AlwaysAutoResize);
	SetTitle("Выбор алгоритма");
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

// TODO: Вынести в родительский класс
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
	}

	ImVec2 windowSize = ImGui::GetWindowSize();
	if (windowPos.x > (Engine::ScreenParams::Width() - windowSize.x)) {
		windowPos.x = Engine::ScreenParams::Width() - windowSize.x;
	}
	if (windowPos.y > (Engine::ScreenParams::Height() - windowSize.y)) {
		windowPos.y = Engine::ScreenParams::Height() - windowSize.y;
	}

	ImGui::SetWindowPos(windowPos);
}

void AlgorithmWindow::OnClose()
{
}

void AlgorithmWindow::Render() {
	auto& space = SpaceManager::Current();

	size_t spaceHash = typeid(*SpaceManager::CurrentPtr().get()).hash_code();
	std::string text = TO_STRING("Current:\n{}", space.GetName());
	ImGui::Text(text.c_str());
	ImGui::Separator();

	auto& params = space.params;
	for (auto it = params.begin(); it != params.end(); ++it) {
		ImGuiIdHandler id(&it);
		ImGui::Checkbox(it->second.c_str(), &it->first);
	}


	ButtonRender<Spaces::Default>();
	ButtonRender<MainThread>();
	ButtonRender<MainThreadSpaceOneBlock>();
	//ButtonRender<XXX>();
}
