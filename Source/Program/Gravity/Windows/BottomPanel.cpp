// ◦ Xyz ◦

#include "BottomPanel.h"
#include <imgui.h>
#include <GuiWindow/ImGuiHelp.h>
#include <Files/FileManager.h>
#include <Common/JsonHelper.h>
#include <Screen.h>
#include <StringUtils.h>
#include "../Spaces/SpaceManager.h"
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
    
    // TODO:
    volatile static bool expOffsetIteration = Engine::GetJsonValue<bool>("expOffsetIteration", Engine::Settings::Instance().JsonData("otherSettings"));
    if (expOffsetIteration) {
        static float offsetIteration = std::log(static_cast<float>(SpaceManager::offsetIteration));
        std::string text = TO_STRING("Speed (error): {}", SpaceManager::offsetIteration);

        if (ImGui::SliderFloat("##offset_iteration_slider", &offsetIteration, 10.f, 15, text.c_str())) {
            //LOG("SpaceManager::offsetIteration: {}, offsetIteration: {}", SpaceManager::offsetIteration, offsetIteration);
            SpaceManager::offsetIteration = static_cast<int>(std::expf(offsetIteration - 1));
        }
    }
    else {
        volatile static float minOffset = Engine::GetJsonValue("minOffset", Engine::Settings::Instance().JsonData("BottomPanel"), 1.f);
        volatile static float maxOffset = Engine::GetJsonValue("maxOffset", Engine::Settings::Instance().JsonData("BottomPanel"), 100.f);

        static float offsetIteration = static_cast<float>(SpaceManager::offsetIteration) / 1000.f;
        std::string text = TO_STRING("Speed (error): {}", SpaceManager::offsetIteration / 1000);

        if (ImGui::SliderFloat("##offset_iteration_slider", &offsetIteration, minOffset, maxOffset, text.c_str())) {
            SpaceManager::offsetIteration = static_cast<int>(offsetIteration) * 1000;
        }
    }
    
    ImGui::SameLine();
    {
        static float countOfIteration = std::log(static_cast<float>(SpaceManager::countOfIteration));
        std::string text = TO_STRING("Count iteration: {}", SpaceManager::countOfIteration);

        if (ImGui::SliderFloat("##count_iteration_slider", &countOfIteration, 0.8f, 8, text.c_str())) {
            SpaceManager::countOfIteration = static_cast<int>(std::expf(countOfIteration - 1));
        }
    }
}
