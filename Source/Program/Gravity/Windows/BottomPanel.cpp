// ◦ Xyz ◦

#include "BottomPanel.h"
#include <imgui.h>
#include <GuiWindow/ImGuiHelp.h>
#include <Screen.h>
#include <StringUtils.h>
#include "../Spaces/Space.h"
#include <Log.h>

using namespace Windows;

BottomPanel::BottomPanel(std::string_view name)
	: Engine::GuiWindow(std::string(name))
{
	SetFlags(ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground);

}

void BottomPanel::OnResize()
{
	const float widthWindow = static_cast<float>(Engine::ScreenParams::Width());
	const float heightWindow = static_cast<float>(Engine::ScreenParams::Height());

	constexpr float heightPanel = 50.f;
	constexpr float borderPanel = 15.f;
	ImGui::SetWindowPos(GetTitle().c_str(), { 0.f, heightWindow - (heightPanel + borderPanel) });
	ImGui::SetWindowSize(GetTitle().c_str(), { widthWindow, heightWindow });
}

void BottomPanel::OnOpen()
{
	SetAlpha(0.f);
	ImGui::SetWindowPos(GetTitle().c_str(), { 0.f, 0.f });
	OnResize();
}

void BottomPanel::Render() {
    volatile static float border = 10.f;
    float widthSlider = Engine::ScreenParams::Width() / 2.f;
    widthSlider -= border;

    ImGuiStyle& style = ImGui::GetStyle();
    VariableHandler styleHandler;
    styleHandler.Push(style.FramePadding.y, 18.f);
    styleHandler.Push(style.WindowPadding, { 1.f, 1.f });
    styleHandler.Push(style.GrabMinSize, 100.f);
    styleHandler.Push(style.GrabRounding, 5.f);
    styleHandler.Push(style.FrameRounding, 5.f);

    ImGuiWidthHandler widthHandler(widthSlider);

    if (ImGui::SliderInt("##deltaTime_slider", &Space::deltaTime, 1, 100, "error = %d")) {
        LOG("deltaTime: {}", Space::deltaTime);
    }

    ImGui::SameLine();

    static float countOfIteration = static_cast<float>(Space::countOfIteration);
    std::string text = TO_STRING("Speed: {}", Space::countOfIteration);

    if (ImGui::SliderFloat("##time_speed_slider", &countOfIteration, 0.8f, 8, text.c_str())) {
        Space::countOfIteration = static_cast<int>(std::expf(countOfIteration - 1));
        LOG("countOfIteration: {}", countOfIteration);
    }
}
