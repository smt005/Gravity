// ◦ Xyz ◦

#include "GuiWindows.h"
#include "GuiWindow.h"
#include <imgui.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <Screen.h>

using namespace Engine;

bool GuiWindows::Init(void* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window, true);
	return ImGui_ImplOpenGL3_Init();
}

void GuiWindows::Cleanup()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void GuiWindows::RenderWindows()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    auto it = _windows.begin();
    while (it != _windows.end()) {
        const auto windowPtr = it->second;
        auto& window = *windowPtr;
        ++it;

        if (window.IsVisible()) {
            if (window.IsFullScreen()) {
                ImGui::SetNextWindowPos(ImVec2(0, 0));
                ImGui::SetNextWindowSize(ImVec2(static_cast<float>(ScreenParams::Width()), static_cast<float>(ScreenParams::Height())));
            }

            if (window._alpha < 1.f) {
                ImGui::SetNextWindowBgAlpha(window._alpha);
            }

            ImGui::Begin(window.GetId(), nullptr, window._flags);
            
            if (!window._opened) {
                window.OnOpen();
                window._opened = true;
            }
            
            window.Render();
            ImGui::End();
        }
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    for (auto& window : _windowToClose) {
        window->OnClose();
    }
    _windowToClose.clear();
}

void GuiWindows::CloseWindow(std::string_view name)
{
    CloseWindow(std::string(name));
}

void GuiWindows::CloseWindow(const std::string& name)
{
    const auto it = _windows.find(name);

    if (it != _windows.end()) {
        _windowToClose.emplace_back(std::move(it->second));
        _windows.erase(it);
    }
}

void GuiWindows::ResizeWindows()
{
    for (const auto& [name, window] : _windows) {
        window->Resize();
    }
}

void GuiWindows::UpdateWindows(double dTime)
{
    for (const auto& [name, window] : _windows) {
        window->Update(dTime);
    }
}

bool GuiWindows::ExistWindow(std::string_view name)
{
    return ExistWindow(std::string(name));
}

bool GuiWindows::ExistWindow(const std::string& name)
{
    return _windows.contains(name);
}

const GuiWindow::Ptr& GuiWindows::Get(const std::string name)
{
    const auto it = _windows.find(name);
    if (it == _windows.end()) {
        throw(TO_STRING("GuiWindow::Get window by name: {} no founded.", name));
    }
    return it->second;
}
