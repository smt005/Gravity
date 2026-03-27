// ◦ Xyz ◦

#include "BottomPanel.h"
#include <imgui.h>
#include <GuiWindow/ImGuiHelp.h>
#include <Files/FileManager.h>
#include <Common/JsonHelper.h>
#include <Screen.h>
#include <StringUtils.h>
#include "../Spaces/SpaceManager.h"
#include <Logs.h>

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
    constexpr float border = 10.f;
    float widthSlider = Engine::ScreenParams::Width() / 2.f;
    widthSlider -= border;

    ImGuiStyle& style = ImGui::GetStyle();
    Engine::VariableHandler styleHandler(style.FramePadding.y, 18.f,
                                        style.WindowPadding, ImVec2(1.f, 1.f),
                                        style.GrabMinSize, 100.f,
                                        style.GrabRounding, 5.f,
                                        style.FrameRounding, 5.f);

    ImGuiWidthHandler widthHandler(widthSlider);
    {
        volatile static float minOffset = Engine::GetJsonValue("minOffset", Engine::Settings::Instance().JsonData("BottomPanel"), 10.f);
        volatile static float maxOffset = Engine::GetJsonValue("maxOffset", Engine::Settings::Instance().JsonData("BottomPanel"), 100000.f);

        int offsetIteration = static_cast<int>(SpaceManager::offsetIteration.load());
        std::string text = TO_STRING("Velocity (error): {}", offsetIteration);

        if (ImGui::SliderInt("##offset_iteration_slider", &offsetIteration, minOffset, maxOffset, text.c_str())) {
            SpaceManager::offsetIteration.store(offsetIteration);
        }
    }

    ImGui::SameLine();
    {
        constexpr float minValue = 0.f;
        constexpr float maxValue = 6.f; // 402

        float countOfIteration = std::log(SpaceManager::countOfIteration.load() + 1);
        const std::string text = TO_STRING("Count iteration: {}", static_cast<int>(SpaceManager::countOfIteration));

        if (ImGui::SliderFloat("##count_iteration_slider", &countOfIteration, minValue, maxValue, text.c_str())) {
            SpaceManager::countOfIteration.store(std::round(std::expf(countOfIteration) - 1));
        }
    }
}
