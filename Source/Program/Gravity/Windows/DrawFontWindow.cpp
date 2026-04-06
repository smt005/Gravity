// ◦ Xyz ◦

#include "DrawFontWindow.h"
#include <imgui.h>
#include <GuiWindow/GuiWindows.h>
#include <GuiWindow/ImGuiHelp.h>
#include <Screen.h>
#include <StringUtils.h>
#include <GuiWindow/ImGuiHelp.h>
#include <Files/FileManager.h>
#include <Files/Settings.h>
#include <Common/JsonHelper.h>
#include <Common/Common.h>
#include <Logs.h>
#include "../Cameras/GravityCameras.h"
#include "../Spaces/SpaceManager.h"
#include "../Spaces/Generators/Generetors.h"

using namespace Windows;

DrawFontWindow::DrawFontWindow(std::string_view name)
	: Engine::GuiWindow(std::string(name))
{
	//SetFlags(ImGuiWindowFlags_AlwaysAutoResize);
	SetTitle("Отображение шрифтов");
	CloseButton(true);

	auto fonts = Engine::FileManager::Get("base").GetFilesRecursive("fonts");
	if (fonts.empty()) {
		return;
	}

	auto fsDir = Engine::FileManager::Get("base").GetRoot() / "Fonts";
	std::string dir = fsDir.string();

	for (auto& font : fonts) {
		std::string fontStr = font.string();
		auto font = fontStr.substr(dir.size() + 1, fontStr.size() - (dir.size() + 1));
		_bufferFonts.emplace_back(font);
	}

	_fonts.resize(fonts.size());
	size_t i = 0;

	for (auto& font : _bufferFonts) {
		_fonts[i] = font.data();
		++i;
	}

	_text[0] = 'T';
	_text[1] = 'e';
	_text[2] = 's';
	_text[3] = 't';
	_text[4] = 'Т';
	_text[5] = 'е';
	_text[6] = 'с';
	_text[7] = 'т';
	_text[8] = '\0';
}


void DrawFontWindow::Render()
{
	if (_bufferFonts.empty()) {
		ImGui::Text("Not fonts!");
		return;
	}

	const float inputWidth = ImGui::GetWindowSize().x - 50.f;
	ImGuiWidthHandler width(inputWidth);

	static int currentIndexFont = 0;

	if (ImGui::InputInt("size", &_fontSize, 1, 26)) {
		_currentFont = GetFont(_fonts[currentIndexFont], _fontSize);
	}
	ImGui::Dummy(ImVec2(0.f, 0.f));

	
	if (ImGui::ListBox("fonts", &currentIndexFont, _fonts.data(), _fonts.size()), 300) {
		_currentFont = GetFont(_fonts[currentIndexFont], _fontSize);
	}
	ImGui::Dummy(ImVec2(0.f, 10.f));

	/*{
		ImGui::Text("Default font");
		ImGui::Separator();

		ImGui::InputText("##def_text", _text.data(), _text.size());
		ImGui::Dummy(ImVec2(0.f, 0.f));

		ImGui::Text("%s", _text.data());
		ImGui::Dummy(ImVec2(0.f, 10.f));
	}

	{
		ImGui::Separator();
		ImGuiFontHandler fontHandler(static_cast<ImFont*>(_currentFont));

		ImGui::InputText("##text", _text.data(), _text.size());
		ImGui::Dummy(ImVec2(0.f, 0.f));

		ImGui::Text("%s", _text.data());
		ImGui::Dummy(ImVec2(0.f, 10.f));
	}*/

	{
		ImGui::Text("Default font");
		ImGui::Separator();

		ImGuiWidthHandler width(200.f);

		ImGui::InputText("##def_text", _text.data(), _text.size());
		ImGui::SameLine();
		{
			ImGuiFontHandler fontHandler(static_cast<ImFont*>(_currentFont));
			ImGui::InputText("##text", _text.data(), _text.size());
		}

		ImGui::Dummy(ImVec2(0.f, 0.f));
		ImGui::Text("%s", _text.data());
		ImGui::SameLine();
		{
			ImGuiFontHandler fontHandler(static_cast<ImFont*>(_currentFont));
			ImGui::Text("%s", _text.data());
		}
	}
}

void* DrawFontWindow::GetFont(char* fileFontName, float fontSize)
{
	const std::string keyFont = TO_STRING("{}_{}", fileFontName, fontSize);
	if (_fontMap.contains(keyFont)) {
		return _fontMap[keyFont];
	}

	auto& fileManager = Engine::FileManager::Get("base");
	auto fontFileNamePath = Engine::FileManager::Get("base").GetRoot() / "Fonts" / fileFontName;
	ImFont* font = nullptr;

	if (std::filesystem::exists(fontFileNamePath)) {
		ImGuiIO& io = ImGui::GetIO();
		ImFontConfig fontConfig;
		//fontConfig.OversampleH = 3;
		//fontConfig.OversampleV = 1;
		//fontConfig.PixelSnapH = true;
		//fontConfig.GlyphRanges = io.Fonts->GetGlyphRangesCyrillic();
		//io.Fonts->AddFontFromFileTTF(fontFileNamePath.string().c_str(), sizeFont, &fontConfig, fontConfig.GlyphRanges);

		font = io.Fonts->AddFontFromFileTTF(fontFileNamePath.string().c_str(), fontSize, &fontConfig, io.Fonts->GetGlyphRangesCyrillic());
		if (font) {
			_fontMap[keyFont] = font;
			LOG("[DrawFontWindow::SetFont] load font: {} size: {}", fileFontName, fontSize);
		}
		else {
			LOG("[DrawFontWindow::SetFont] FAIL load font: {} size: {}", fileFontName, fontSize);
		}
	}

	return font;
}
