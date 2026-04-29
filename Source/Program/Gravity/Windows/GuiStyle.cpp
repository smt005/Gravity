// ◦ Xyz ◦

#include "GuiStyle.h"
#include <string>
#include <imgui.h>
#include <Files/FileManager.h>
#include <Files/Settings.h>

void Windows::LoadGuiStyle()
{
    bool styleColorsDark = true;
    std::string fontFileName = "Default.ttf";
    float sizeFont = 14.f;
    float spaceX = 1.f;

    if (const Json* guiWindowsSettings = Engine::Settings::Instance().JsonData("GuiWindows")) {
        const std::string style = Engine::GetJsonValue("style", guiWindowsSettings, std::string("Dark")); // Light Dark
        styleColorsDark = style == "Dark";

        fontFileName = Engine::GetJsonValue("font", guiWindowsSettings, fontFileName);
        sizeFont = Engine::GetJsonValue("sizeFont", guiWindowsSettings, 12.f);
        if (sizeFont < 1.f) {
            sizeFont = 10.f;
        }
        spaceX = Engine::GetJsonValue("spaceX", guiWindowsSettings, 1.f);
        if (spaceX < 0.f) {
            spaceX = 0.f;
        }
    }

    auto& fileManager = Engine::FileManager::Get("base");
    auto fontFileNamePath = Engine::FileManager::Get("base").GetRoot() / "Fonts" / fontFileName;

    if (std::filesystem::exists(fontFileNamePath)) {
        ImGuiIO& io = ImGui::GetIO();
        ImFontConfig fontConfig;
        fontConfig.GlyphExtraAdvanceX = spaceX;
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
