// ◦ Xyz ◦

#include "TopPanel.h"
#include <imgui.h>
#include <Screen.h>
#include <StringUtils.h>
#include <GuiWindow/GuiWindows.h>
#include "../DebugContext.h"
#include <Logs.h>

using namespace Windows;

TopPanel::TopPanel(std::string_view name)
	: Engine::GuiWindow(std::string(name))
{
	SetFlags(ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground);

}

void TopPanel::OnResize()
{
	const float width = static_cast<float>(Engine::ScreenParams::Width());
	const float height = static_cast<float>(Engine::ScreenParams::Height());

	ImGui::SetWindowPos(GetTitle().c_str(), { 0.f, 0.f });
	ImGui::SetWindowSize(GetTitle().c_str(), { width, 30.f });
}

void TopPanel::OnOpen()
{
	SetAlpha(0.f);
	ImGui::SetWindowPos(GetTitle().c_str(), { 0.f, 0.f });
	OnResize();
}

static ImVec4 GetCollor(double ups) {
	ImVec4 color = { 1.f, 1.f, 1.f, 1.f };
	if (ups < 10) {
		color = { 1.f, 0.f, 0.f, 1.f };
	}
	if (ups >= 30) {
		color = { 0.f, 1.f, 0.f, 1.f };
	}
	return color;
}

void TopPanel::Render() {
	{
		std::string_view prefix;
#ifdef _DEBUG
		prefix = "DEBUG";
#endif
		std::string text = TO_STRING("{} fps: {} ", prefix, static_cast<int>(_fps));
		ImGui::TextColored(GetCollor(_fps), text.c_str());
	}
	
	{
		std::string text;
		if (_ups > 1) {
			text = TO_STRING("\tups: {} ", static_cast<int>(_ups));
		}
		else {
			text = TO_STRING("\tups: {}/1000", static_cast<int>(_ups * 1000));
		}
		ImGui::SameLine();
		ImGui::TextColored(GetCollor(_ups), text.c_str());
	}
}

void TopPanel::Update(double dTime) {
	_fps = 1 / dTime;

	const double updateDeltaTime = DebugContext::Instance().updateDeltaTime.load();
	_ups = 1 / updateDeltaTime;
}
