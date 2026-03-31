// ◦ Xyz ◦

#include "RenderWindow.h"
#include <imgui.h>
#include <GuiWindow/GuiWindows.h>
#include <GuiWindow/ImGuiHelp.h>
#include <Screen.h>
#include <StringUtils.h>
#include <GuiWindow/ImGuiHelp.h>
#include <Files/FileManager.h>
#include <Files/Settings.h>
#include <Common/JsonHelper.h>
#include <Common/Common.h>
#include <Logs.h>
#include "../Cameras/GravityCameras.h"
#include "../Render/GravityRender.h"
#include "../Spaces/SpaceManager.h"
#include "../Spaces/Generators/Generetors.h"

using namespace Windows;

RenderWindow::RenderWindow(std::string_view name)
	: Engine::GuiWindow(std::string(name))
{
	SetFlags(ImGuiWindowFlags_AlwaysAutoResize);
	SetTitle("Render");
	CloseButton(true);
}


void RenderWindow::Render()
{
	ImGuiWidthHandler width(100.f);

	ImGui::SliderFloat("scale bosy", &GravityRender::scaleBody, 1.f, 10.f);
	ImGui::Dummy(ImVec2(0.f, 10.f));

	ImGui::Checkbox("show body as point", &GravityRender::typeDraw.point);
	ImGui::Dummy(ImVec2(0.f, 10.f));
}
