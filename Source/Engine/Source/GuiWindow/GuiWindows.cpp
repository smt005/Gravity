// ◦ Xyz ◦

#include "GuiWindows.h"
#include "GuiWindow.h"
#include <exception>
#include <imgui.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <Screen.h>
#include <Files/FileManager.h>
#include <Files/Settings.h>
#include <Common/JsonHelper.h>

using namespace Engine;

bool GuiWindows::Init(void* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

    LoadStyle();


	ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window, true);
	return ImGui_ImplOpenGL3_Init();
}

void GuiWindows::LoadStyle()
{
    bool styleColorsDark = true;
    std::string fontFileName = "DefaultFont.ttf";
    float sizeFont = 13.f;

    if (const Json* guiWindowsSettings = Settings::Instance().JsonData("GuiWindows")) {
        const std::string style = Engine::GetJsonValue("style", guiWindowsSettings, std::string("Dark")); // Light
        styleColorsDark = style == "Dark";

        fontFileName = Engine::GetJsonValue("font", guiWindowsSettings, fontFileName);
        sizeFont = Engine::GetJsonValue("sizeFont", guiWindowsSettings, 12.f);
        if (sizeFont < 1.f) {
            sizeFont = 10.f;
        }
    }

    auto& fileManager = Engine::FileManager::Get("base");
    auto fontFileNamePath = Engine::FileManager::Get("base").GetRoot() / "Fonts" / fontFileName;

    if (std::filesystem::exists(fontFileNamePath)) {
        ImGuiIO& io = ImGui::GetIO();
        ImFontConfig fontConfig;
        io.Fonts->AddFontFromFileTTF(fontFileNamePath.string().c_str(), sizeFont, &fontConfig, io.Fonts->GetGlyphRangesCyrillic());

        LOG("[GuiWindows::Init] load font: {} size: {}", fontFileName, sizeFont);
    }
    
    if (styleColorsDark) {
        ImGui::StyleColorsDark();
    }
    else {
        ImGui::StyleColorsLight();
    }
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

        if (window.IsVisible()) {
            if (window.IsFullScreen()) {
                ImGui::SetNextWindowPos(ImVec2(0, 0));
                ImGui::SetNextWindowSize(ImVec2(static_cast<float>(ScreenParams::Width()), static_cast<float>(ScreenParams::Height())));
            }

            ImGui::SetNextWindowBgAlpha(window._alpha);

            ImGui::Begin(window.GetId(), window._openPtr.get(), window._flags);

            if (_windowsOpen.contains(window.GetName())) {
                window.OnOpen();
            }

            if (_allWindowsResize) {
                window.OnResize();
            }
            else {
                if (_windowsResize.contains(window.GetName())) {
                    window.OnResize();
                }
            }

            if (window._openPtr && !*window._openPtr) {
                window.OnClose();
                it = _windows.erase(it);
            }
            else {
                ++it;
            }

            window.Render();
            ImGui::End();
        }
    }
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    _allWindowsResize = false;
    _windowsResize.clear();
    _windowsOpen.clear();
}

void GuiWindows::CloseWindow(std::string_view name)
{
    CloseWindow(std::string(name));
}

void GuiWindows::CloseWindow(const std::string& name)
{
    const auto it = _windows.find(name);

    if (it != _windows.end()) {
        it->second->_openPtr = mystd::make_shared<bool>(false);
    }
}

void GuiWindows::CloseWindows()
{
    for (const auto& [name, window] : _windows) {
        window->_openPtr = mystd::make_shared<bool>(false);
    }
}

void GuiWindows::ResizeWindows(std::string_view name)
{
    if (name.empty()) {
        _allWindowsResize = true;
    }
    else {
        _windowsResize.emplace(name);
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
