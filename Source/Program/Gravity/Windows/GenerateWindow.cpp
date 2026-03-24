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
#include <Common/JsonHelper.h>
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
	const static ImVec2 buttonSize(200.f, 20.f);

	{
		static int count = 0;
		static  int minSize = 100;
		static  int size = 100;

		if (!count || !size) {
			count = SpaceManager::GetGenerateValue("count", 200);
			size = SpaceManager::GetGenerateValue("size", 200);
			minSize = SpaceManager::GetGenerateValue("minSize", 1);

			if (count == 0) {
				LOG("[GenerateWindow::Render] count: {}", count);
				count = 200;
			}
			if (size == 0) {
				LOG("[GenerateWindow::Render] size: {}", size);
				size = 200;
			}
		}

		ImGuiWidthHandler width(100.f);
		ImGui::InputInt("count", &count, 1, 100000);
		ImGui::InputInt("min size", &minSize, 1, 100000);
		ImGui::InputInt("size", &size, 1, 100000);

		if (ImGui::Button("Box", buttonSize)) {
			SpaceManager::Generate(SpaceManager::GenerateType::BOX, count, minSize, size);
		}

		if (ImGui::Button("Round plane", buttonSize)) {
			SpaceManager::Generate(SpaceManager::GenerateType::BOX, count, minSize, size);
		}

		if (ImGui::Button("On plane orbit", buttonSize)) {
			SpaceManager::Generate(SpaceManager::GenerateType::ORBIT_ON_MAIN_BODT, count, minSize, size);
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
