// ◦ Xyz ◦
#pragma once

#include <GuiWindow/GuiWindow.h>
#include <string>
#include <functional>
#include <GuiWindow/ImGuiHelp.h>
#include <Common/Common.h>
#include "../Spaces/BodyData.h"
#include "../Spaces/SpaceManager.h"

namespace Windows
{
	class AlgorithmWindow final : public Engine::GuiWindow
	{
	public:
		AlgorithmWindow(std::string_view name);
		~AlgorithmWindow() = default;
		void Render() override;
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

			std::string nameSpace = Engine::ExtractClassName(typeid(T).name());
			ImGuiColorHandler color(ImGuiCol_Button, _currentSpace == nameSpace ? currentColor : defaultColor);

			if (ImGui::Button(nameSpace.c_str(), buttonSize)) {
				SpaceManager::SwitchSpace<T>();
				_currentSpace = nameSpace;
			}

		}

	private:
		std::string _currentSpace;

	public:
		inline static const std::string_view windowName = "AlgorithmWindow";
	};
}
