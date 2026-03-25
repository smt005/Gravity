// ◦ Xyz ◦

#include "DebugWindow.h"
#include "../Spaces/SpaceManager.h"
#include "../DebugContext.h"
#include <Draw/Camera.h>
#include <GuiWindow/GuiWindows.h>
#include <imgui.h>
#include <Screen.h>
#include <StringUtils.h>
#include <Logs.h>

using namespace Windows;

DebugWindow::DebugWindow(std::string_view name)
	: Engine::GuiWindow(std::string(name))
{
	SetFlags(ImGuiWindowFlags_AlwaysAutoResize);
	SetTitle("Debud info");
}

void DebugWindow::OnOpen()
{
	//const float width = static_cast<float>(Engine::ScreenParams::Width());
	ImGui::SetWindowPos(GetId(), { 10.f, 36.f });
	ImGui::SetWindowSize(GetId(), { 200.f, 300.f });

	LOG("DebugWindow::OnOpen");
}

void DebugWindow::OnClose()
{
	LOG("DebugWindow::OnClose");
}

void DebugWindow::Render() {
	DebugContext& debugContext = DebugContext::Instance();

	ImGui::Text("  Count object: %d", debugContext.countObject);
	ImGui::Text("Diameter space: %f", debugContext.diameterSpace);
	
	ImGui::Separator();
	ImGui::Text("     Min force: %f", debugContext.minForce * 1000000.f);
	ImGui::Text("     Max force: %f", debugContext.maxForce * 1000000.f);
	
	ImGui::Separator();
	ImGui::Text("     Min speed: %f", debugContext.minVelocity * 1000000.f);
	ImGui::Text("     Max speed: %f", debugContext.maxVelocity * 1000000.f);
	
	ImGui::Separator();
	ImGui::Text("      Min mass: %f", debugContext.minMass);
	if (ImGui::IsItemHovered()) {
		if (ImGui::IsItemClicked()) {
			CameraToMinMassObject();
		}
	}

	ImGui::Text("      Max mass: %f", debugContext.maxMass);
	if (ImGui::IsItemHovered()) {
		if (ImGui::IsItemClicked()) {
			CameraToMaxMassObject();
		}
	}

	ImGui::Text("      Sum mass: %f", debugContext.sumMass);

	ImGui::Separator();
	ImGui::Text("   Const force: %f", debugContext.constForce);
	ImGui::Text("   Const speed: %f", debugContext.constSpeed);

	if (debugContext.subProgress >= 0) {
		char buf[32];
		sprintf(buf, "sub: %d/%d", (int)(debugContext.subProgress * 100), 100);
		ImGui::ProgressBar(debugContext.subProgress, ImVec2(0.f, 0.f), buf);
	}
	if (debugContext.progress >= 0) {
		char buf[32];
		sprintf(buf, "%d/%d", (int)(debugContext.progress * 100), 100);
		ImGui::ProgressBar(debugContext.progress, ImVec2(0.f, 0.f), buf);
	}
}

void DebugWindow::Update(double dTime) {
	if (timeOut > 0 || dTime == 0) {
		timeOut -= dTime;
		return;
	}

	_fps = 1 / dTime;
	_minFps = std::min(_fps, _minFps);
	_maxFps = std::max(_fps, _maxFps);
}

void DebugWindow::CameraToMinMassObject()
{
	Engine::Camera::GetLink().SetPos(SpaceManager::PosOfMinMassObject());
}

void DebugWindow::CameraToMaxMassObject()
{
	Engine::Camera::GetLink().SetPos(SpaceManager::PosOfMaxMassObject());
}
