// ◦ Xyz ◦
#include "GuiWindow.h"
#include "GuiWindows.h"
#include <imgui.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <Screen.h>

using namespace Engine;

GuiWindow::GuiWindow()
    : _name(std::to_string(reinterpret_cast<uint64_t>(this)))
    , _title(TO_STRING("##{}", _name))
{}

GuiWindow::GuiWindow(const std::string& name)
    : _name(name)
    , _title(TO_STRING("{}##{}", name, _name))
{}

const std::string& GuiWindow::GetName() const
{
	return _name;
}

const std::string& GuiWindow::GetTitle() const
{
    return _title;
}

void GuiWindow::SetTitle(std::string_view title)
{
    _title = TO_STRING("{}##{}", title, _name);
}

bool GuiWindow::IsVisible() const
{
    return _visible;
}

void GuiWindow::SetVisible(bool state)
{
    _visible = state;
}

int GuiWindow::GetFlags() const
{
    return _flags;
}

void GuiWindow::SetFlags(int flag)
{
    _flags = flag;
}

int GuiWindow::GetAlpha() const
{
    return _alpha;
}

void GuiWindow::SetAlpha(int value)
{
    _alpha = value;
}

bool GuiWindow::IsFullScreen() const
{
    return _fullScreen;
}

void GuiWindow::SetFullScreen(bool state)
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

    GuiWindows::ResizeWindows(_name);
}

void GuiWindow::Close()
{
    GuiWindows::CloseWindow(GetName());
}

const char* GuiWindow::GetId() const
{
    return GetTitle().c_str();
}

void GuiWindow::CloseButton(bool show)
{
    if (show) {
        _openPtr = mystd::make_shared<bool>(true);
    }
    else {
        _openPtr.reset();
    }
}
