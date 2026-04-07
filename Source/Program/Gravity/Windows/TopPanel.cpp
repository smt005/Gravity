// ◦ Xyz ◦

#include "TopPanel.h"
#include <imgui.h>
#include <Screen.h>
#include <StringUtils.h>
#include <GuiWindow/GuiWindows.h>
#include <Logs.h>
#include "../DebugContext.h"
#include "Common/Common.h"

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
		std::string text = TO_STRING("{} Time: {} ", prefix, Engine::ValueToString(Engine::Callback::TimePassed() / 1000, 0));
		ImGui::Text(text.c_str());
		ImGui::SameLine();
	}

	{
		std::string text = TO_STRING("fps: {} ({})", _roundFps, Engine::ValueToString(_fps));
		ImGui::TextColored(GetCollor(_fps), text.c_str());
		ImGui::SameLine();
	}

	DebugContext& debugContext = DebugContext::Instance();
	const size_t size = debugContext.deltaTimes.size();

	if (size > 0) {	
		std::string times = " [";

		for (size_t i = 0; i < size; ++i) {
			if (debugContext.deltaTimes[i] < 10) {
				times += TO_STRING("{}: 0 (∞), ", i);
			}
			else {
				times += TO_STRING("{}: {} ({}), ", i, Engine::ValueToString(debugContext.deltaTimes[i]), Engine::ValueToString(1 / debugContext.deltaTimes[i] * 1000));
			}
		}
		times += "] ";

		ImGui::Text(times.c_str());
		ImGui::SameLine();
	}
}

void TopPanel::Update(double deltaTime) {
	_fps = 1 / deltaTime;

	_sumTimeFps += deltaTime;
	++_countFrame;

	if (_sumTimeFps >= 1) {
		_roundFps = _countFrame;
		_countFrame = 0;
		_sumTimeFps = 0;
	}
}
