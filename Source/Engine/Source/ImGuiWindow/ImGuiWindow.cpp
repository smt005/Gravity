// ◦ Xyz ◦
#include "ImGuiWindow.h"
#include <imgui.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <Screen.h>

using namespace Engine;

ImGuiWindow::ImGuiWindow()
    : _name(std::to_string(reinterpret_cast<uint64_t>(this)))
    , _title(TO_STRING("##{}", _name))
{}

ImGuiWindow::ImGuiWindow(const std::string& name)
    : _name(name)
    , _title(TO_STRING("##{}", name, std::to_string(reinterpret_cast<uint64_t>(this))))
{}

const std::string& ImGuiWindow::GetName() const
{
	return _name;
}

const std::string& ImGuiWindow::GetTitle() const
{
    return _title;
}

void ImGuiWindow::SetTitle(std::string_view title)
{
    _title = title;
}

bool ImGuiWindow::IsVisible() const
{
    return _visible;
}

void ImGuiWindow::SetVisible(bool state)
{
    _visible = state;
}

bool ImGuiWindow::IsFullScreen() const
{
    return _fullScreen;
}

void ImGuiWindow::SetFullScreen(bool state)
{
    _fullScreen = state;

    if (_fullScreen) {
        constexpr ImGuiWindowFlags fullScreenFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus;
        _flags = _flags | fullScreenFlags;
    }
    else {
        constexpr ImGuiWindowFlags noFullScreenFlags = ImGuiWindowFlags_NoTitleBar;
        _flags = _flags ^ noFullScreenFlags;
    }

    Resize(); // TODO:
}

void ImGuiWindow::Close()
{
	CloseWindow(GetName());
}

bool ImGuiWindow::Init(void* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window, true);
	return ImGui_ImplOpenGL3_Init();
}

void ImGuiWindow::Cleanup()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiWindow::RenderWindows()
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
                ImGui::SetNextWindowSize(ImVec2(static_cast<float>(ScreenParams::width), static_cast<float>(ScreenParams::height)));
            }
            
            ImGui::Begin(window.GetTitle().c_str(), nullptr, window._flags);
            
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

void ImGuiWindow::CloseWindow(const std::string& name)
{
    const auto it = _windows.find(name);

    if (it != _windows.end()) {
        _windowToClose.emplace_back(std::move(it->second));
        _windows.erase(it);
    }
}

void ImGuiWindow::ResizeWindows()
{
    for (const auto& [name, window] : _windows) {
        window->Resize();
    }
}

void ImGuiWindow::UpdateWindows(double dTime)
{
    for (const auto& [name, window] : _windows) {
        window->Update(dTime);
    }
}

bool ImGuiWindow::ExistWindow(const std::string& name)
{
    return _windows.contains(name);
}

const ImGuiWindow::Ptr& ImGuiWindow::Get(const std::string name)
{
    const auto it = _windows.find(name);
    if (it == _windows.end()) {
        throw(TO_STRING("ImGuiWindow::Get window by name: {} no founded.", name));
    }
    return it->second;
}
