// ◦ Xyz ◦

#pragma once

#include <vector>
#include <imgui.h>

struct ImGuiFontHandler final {
    ImGuiFontHandler(ImFont* _font)
        : font(_font)
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

struct ImGuiColorScopeHandler final {
    template <typename ...Args>
    ImGuiColorScopeHandler(Args&&... args)
    {
        Add(std::forward<Args>(args)...);
    }

    ~ImGuiColorScopeHandler()
    {
        ImGui::PopStyleColor(countPushed);
    }

private:
    void Add(ImGuiCol idx, ImVec4&& color) {
        ImGui::PushStyleColor(idx, std::forward<ImVec4>(color));
        ++countPushed;
    }
    
    template <typename ...Args>
    void Add(ImGuiCol idx, ImVec4&& color, Args&&... args) {
        ImGui::PushStyleColor(idx, std::forward<ImVec4>(color));
        ++countPushed;
        Add(std::forward<Args>(args)...);
    }

    size_t countPushed = 0;
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
