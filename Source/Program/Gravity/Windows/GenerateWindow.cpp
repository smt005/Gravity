// ◦ Xyz ◦

#include "GenerateWindow.h"
#include <GuiWindow/GuiWindows.h>
#include <imgui.h>
#include <Screen.h>
#include <StringUtils.h>
#include <GuiWindow/ImGuiHelp.h>
#include "../Spaces/SpaceManager.h"
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
		ImGuiWidthHandler width(100.f);

		static std::string countStr(128, '\0');
		ImGui::InputText("count", countStr.data(), countStr.size() - 1);

		static std::string radiusStr(128, '\0');
		ImGui::InputText("radius", radiusStr.data(), radiusStr.size() - 1);

		if (ImGui::Button("Round plane", buttonSize)) {
			SpaceManager::Current().Generate(300, 50);
		}

		ImGui::Separator();
	}

	for (int i = 0; i < 5; ++i) {
		if (ImGui::Button(TO_STRING("{}", i).c_str(), buttonSize)) {
			LOG("BUTTON: {}", i);
		}

		ImGui::Separator();
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
