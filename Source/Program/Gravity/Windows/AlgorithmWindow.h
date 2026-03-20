// ◦ Xyz ◦
#pragma once

#include <GuiWindow/GuiWindow.h>
#include <string>
#include <functional>
#include <GuiWindow/ImGuiHelp.h>
#include <Common/Common.h>
#include "../Spaces/Space.h"
#include "../Spaces/OneThreadSpace.h"
#include "../Spaces/SpaceManager.h"

namespace Windows
{
	class AlgorithmWindow final : public Engine::GuiWindow
	{
	public:
		AlgorithmWindow(std::string_view name);
		~AlgorithmWindow() = default;
		void Render() override;
		void Update(double dTime) override;
		void OnResize() override;
		void OnOpen() override;
		void OnClose() override;

	private:
		void FixSize();
		
		template <typename T>
		void ButtonRender() {
			const static ImVec2 buttonSize(200.f, 20.f);
			const static ImColor currentColor(0.9f, 0.6f, 0.3f, 1.f);
			const static ImColor defaultColor(0.5f, 0.6f, 0.7f, 1.f);

			ImGuiColorHandler color(ImGuiCol_Button, _currentSpace == typeid(T).hash_code() ? currentColor : defaultColor);
			std::string nameBtn = Engine::ExtractClassName(typeid(T).name());

			if (ImGui::Button(nameBtn.data(), buttonSize)) {
				SpaceManager::SetCurrent<T>() = SpaceManager::Current();
				_currentSpace = typeid(T).hash_code();
			}

		}

	public:
		static void SwitchVisibleWindow();
		static bool OpenWindow();
		static void CloseWindow();

	private:
		size_t _currentSpace = 0;

	public:
		inline static const std::string_view windowName = "AlgorithmWindow";
	};
}
