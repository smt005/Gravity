// ◦ Xyz ◦

#pragma once

#include <imgui.h>
#include <WindowsManager.h>

struct ImGuiFontHandler final {
    ImGuiFontHandler(ImFont* _font)
        : font(_font)
    {
        if (font) {
            ImGui::PushFont(font);
        }
    }

    ImGuiFontHandler(int size)
        : font(WindowsManager::GetFont(size))
    {
        if (font) {
            ImGui::PushFont(font);
        }
    }

    ~ImGuiFontHandler()
    {
        if (font) {
            ImGui::PopFont();
        }
    }

    ImFont* font;
};

struct ImGuiIdHandler final {
    ImGuiIdHandler(void* _id)
        : id(_id)
    {
        if (id) {
            ImGui::PushID(id);
        }
    }

    ~ImGuiIdHandler()
    {
        if (id) {
            ImGui::PopID();
        }
    }

    void* id;
};

struct ImGuiColorHandler final {
    ImGuiColorHandler(ImGuiCol idx, const ImVec4& color)
    {
        ImGui::PushStyleColor(idx, color);
    }

    ~ImGuiColorHandler()
    {
        ImGui::PopStyleColor();
    }
};

struct ImGuiWidthHandler final {
    ImGuiWidthHandler(float width)
    {
        ImGui::PushItemWidth(width);
    }

    ~ImGuiWidthHandler()
    {
        ImGui::PopItemWidth();
    }
};
