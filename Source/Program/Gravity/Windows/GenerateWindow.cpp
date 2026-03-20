// ◦ Xyz ◦

#include "GenerateWindow.h"
#include <GuiWindow/GuiWindows.h>
#include <imgui.h>
#include <Screen.h>
#include <StringUtils.h>
#include <GuiWindow/ImGuiHelp.h>
#include <Files/Settings.h>
#include "../Spaces/SpaceManager.h"
#include "../Cameras/GravityCameras.h"
#include <Common/Common.h>
#include <Log.h>

using namespace Windows;

GenerateWindow::GenerateWindow(std::string_view name)
	: Engine::GuiWindow(std::string(name))
{
	SetFlags(ImGuiWindowFlags_AlwaysAutoResize);
	SetTitle("Generate");
	CloseButton(true);
}

void GenerateWindow::OnResize()
{
	FixSize();
}

void GenerateWindow::OnOpen()
{
	OnResize();
}

void GenerateWindow::Render() {
	const static ImVec2 buttonSize(150.f, 20.f);

	{
		// TODO:
		static int count = 0;
		static  int size = 100;

		if (!count || !size) {
			const auto& settings = Engine::Settings::Instance();
			if (auto* jsonData = settings.JsonData(TO_STRING("{}/generateData", Engine::GetClassName<SpaceManager>()))) {
				count = (*jsonData)["count"];
				size = (*jsonData)["size"];

				if (count == 0) {
					count = 100;
				}
				if (size == 0) {
					size = 100;
				}
			}
		}

		ImGuiWidthHandler width(100.f);
		ImGui::InputInt("count", &count, 1, 100000);
		ImGui::InputInt("size", &size, 1, 100000);

		if (ImGui::Button("Box", buttonSize)) {
			SpaceManager::Current().Generate(count, size, 0);
		}

		if (ImGui::Button("Round plane", buttonSize)) {
			SpaceManager::Current().Generate(count, size, 1);
		}

		ImGui::Separator();
	}

	ImGui::Separator();
	if (ImGui::Button("Reset camera", buttonSize)) {
		cameras::ResetCamera();
	}

}

void GenerateWindow::FixSize()
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

void GenerateWindow::Update(double dTime) {

}

void GenerateWindow::SwitchVisibleWindow()
{
	if (!OpenWindow()) {
		CloseWindow();
	}
}

bool GenerateWindow::OpenWindow()
{
	if (!Engine::GuiWindows::ExistWindow(windowName)) {
		Engine::GuiWindows::MakeWindow<GenerateWindow>(windowName)->GetName();
		return true;
	}

	return false;
}

void GenerateWindow::CloseWindow()
{
	Engine::GuiWindows::CloseWindow(windowName);
}
