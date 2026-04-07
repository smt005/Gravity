// ◦ Xyz ◦

#include "DebugWindow.h"
#include <Draw/Camera.h>
#include <imgui.h>
#include <GuiWindow/ImGuiHelp.h>
#include <GuiWindow/GuiWindows.h>
#include <Screen.h>
#include <StringUtils.h>
#include <Common/Common.h>
#include <Logs.h>
#include "../Spaces/SpaceManager.h"
#include "../DebugContext.h"

using namespace Windows;

DebugWindow::DebugWindow(std::string_view name)
	: Engine::GuiWindow(std::string(name))
{
	SetFlags(ImGuiWindowFlags_AlwaysAutoResize);
	SetTitle("Отладочная информация");
}

void DebugWindow::OnOpen()
{
	ImGui::SetWindowPos(GetId(), { 10.f, 36.f });
	ImGui::SetWindowSize(GetId(), { 200.f, 300.f });

	if (const auto* settings = Engine::Settings::Instance().JsonData("DebugWindow")) {
		bool progressCollapsing = Engine::GetJsonValue<bool>("progressCollapsing", settings, true);
		bool timesCollapsing = Engine::GetJsonValue<bool>("timesCollapsing", settings, false);
	
		ImGui::GetStateStorage()->SetBool(ImGui::GetID("Progress"), progressCollapsing);
		ImGui::GetStateStorage()->SetBool(ImGui::GetID("Time"), timesCollapsing);
	}
}

void DebugWindow::OnClose()
{
	if (auto* settings = Engine::Settings::Instance().JsonData("DebugWindow", true)) {
		(*settings)["progressCollapsing"] = ImGui::GetStateStorage()->GetBool(ImGui::GetID("Progress"));
		(*settings)["timesCollapsing"] = ImGui::GetStateStorage()->GetBool(ImGui::GetID("Time"));
	}
	LOG("[DebugWindow::OnClose] end");
}

void DebugWindow::Render() {
	DebugContext& debugContext = DebugContext::Instance();

	ImGui::Text("  Count object: %d", debugContext.countObject);
	ImGui::Text("Diameter space: %f", debugContext.diameterSpace);
	
	ImGui::Separator();
	//ImGui::Text("     Min force: %f", debugContext.minForce * 1000000.f);
	//ImGui::Text("     Max force: %f", debugContext.maxForce * 1000000.f);
	
	ImGui::Separator();
	ImGui::Text("     Min speed: %f", debugContext.minVelocity * 1000000.f);
	ImGui::Text("     Max speed: %f", debugContext.maxVelocity * 1000000.f);
	
	ImGui::Separator();
	ImGui::Text("      Min mass: %f", debugContext.minMass);
	ImGui::Text("      Max mass: %f", debugContext.maxMass);
	ImGui::Text("      Sum mass: %f", debugContext.sumMass);

	ImGui::Separator();
	ImGui::Text("   Const force: %f", debugContext.constForce);
	ImGui::Text("   Const speed: %f", debugContext.constSpeed);

	if (ImGui::CollapsingHeader("Progress")) {
		ImGuiColorScopeHandler colorhandlers(ImGuiCol_FrameBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f),
		ImGuiCol_PlotHistogram, ImVec4(0.2f, 0.2f, 0.2f, 1.0f),
		ImGuiCol_Text, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));

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

	if (ImGui::CollapsingHeader("Time")) {
		const size_t size = debugContext.deltaTimes.size();
		for (size_t i = 0; i < size; ++i) {
			std::string text;
			if (debugContext.deltaTimes[i] < 10) {
				text = TO_STRING("{}: 0 (∞), ", i);
			}
			else {
				text = TO_STRING("{}: {} ({}), ", i, Engine::ValueToString(debugContext.deltaTimes[i]), Engine::ValueToString(1 / debugContext.deltaTimes[i] * 1000));
			}
			ImGui::Text(text.c_str());
		}
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
