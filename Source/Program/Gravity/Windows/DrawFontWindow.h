// ◦ Xyz ◦
#pragma once

#include <GuiWindow/GuiWindow.h>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <unordered_map>

namespace Windows
{
	class DrawFontWindow final : public Engine::GuiWindow
	{
	public:
		DrawFontWindow(std::string_view name);
		~DrawFontWindow() = default;
		void Render() override;

	private:
		void* GetFont(char* fileFontName, float fontSize);

	private:
		int _fontSize = 13;
		std::vector<char*> _fonts;
		std::list<std::string> _bufferFonts;
		std::array<char, 128> _text;
		void* _currentFont = nullptr;
		std::unordered_map<std::string, void*> _fontMap;

	public:
		inline static const std::string_view windowName = "DrawFontWindow";
	};
}
