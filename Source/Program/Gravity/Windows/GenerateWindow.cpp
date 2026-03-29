// ◦ Xyz ◦

#include "GenerateWindow.h"
#include <GuiWindow/GuiWindows.h>
#include <imgui.h>
#include <Screen.h>
#include <StringUtils.h>
#include <GuiWindow/ImGuiHelp.h>
#include <Files/Settings.h>
#include <Common/JsonHelper.h>
#include <Common/Common.h>
#include <Logs.h>
#include "../Cameras/GravityCameras.h"
#include "../Spaces/SpaceManager.h"
#include "../Spaces/Generators/Generetors.h"

namespace
{
	template <typename TGenerator>
	void ButtonRender() {
		static const ImVec2 buttonSize(200.f, 20.f);
		const std::string lebelButton = Engine::GetClassName<TGenerator>();

		if (ImGui::Button(lebelButton.c_str(), buttonSize)) {
			SpaceManager::GenerateSpace<TGenerator>();
		}
	}
}

template <typename T>
struct Windows::InputValue final
{
	T value;
	const std::string key;

	InputValue<T>(std::string_view _key, T defaultValue) : key(_key) {
		value = Engine::GetJsonValue(key, GeneratorSpace::GetData(), defaultValue);
	}
	~InputValue<T>() = default;

	InputValue<T>(const InputValue<T>&) = delete;
	InputValue<T>(InputValue<T>&&) = delete;
	void operator = (const InputValue<T>&) = delete;
	void operator = (InputValue<T>&&) = delete;

	void Render() {
		bool changed = false;

		if constexpr (std::is_integral_v<T>) {
			changed = ImGui::InputInt(key.c_str(), &value, 0, 10000);
		}
		else if constexpr (std::is_floating_point_v<T>) {
			changed = ImGui::InputFloat(key.c_str(), &value, 0, 10000);
		}

		if (changed) {
			GeneratorSpace::GetData()[key] = value;
		}
	}
};

using namespace Windows;

GenerateWindow::GenerateWindow(std::string_view name)
	: Engine::GuiWindow(std::string(name))
{
	SetFlags(ImGuiWindowFlags_AlwaysAutoResize);
	SetTitle("Generate");
	CloseButton(true);

	_countInput = mystd::make_shared<InputValue<int>>("count", 300);
	_sizeInput = mystd::make_shared<InputValue<int>>("size", 300);
	_minMassInput = mystd::make_shared<InputValue<float>>("minMass", 0.1f);
	_maxMassInput = mystd::make_shared<InputValue<float>>("maxMass", 1.f);
}

void GenerateWindow::OnResize()
{
	FixSize();
}

void GenerateWindow::OnOpen()
{
	OnResize();
}

void GenerateWindow::Render()
{
	ImGuiWidthHandler width(100.f);

	_countInput->Render();
	_sizeInput->Render();
	_minMassInput->Render();
	_maxMassInput->Render();

	ImGui::Separator();

	ButtonRender<GeneratorSpace::Default>();
	ButtonRender<GeneratorSpace::Box>();
	ButtonRender<GeneratorSpace::Circle>();
	ButtonRender<GeneratorSpace::Sphere>();
	ButtonRender<GeneratorSpace::OnPlaneOrbit>();
	ButtonRender<GeneratorSpace::OnOrbit>();
	

	const static ImVec2 buttonSize(200.f, 20.f);

	ImGui::Separator();
	if (ImGui::Button("Reset camera", buttonSize)) {
		cameras::ResetCamera();
	}
}

void GenerateWindow::FixSize()
{
	ImVec2 windowPos = ImGui::GetWindowPos();
	if (windowPos.x < 0 || windowPos.y < 0) {
		if (windowPos.x < 0) {
			windowPos.x = 0;
		}
		if (windowPos.y < 0) {
			windowPos.y = 0;
		}
		ImGui::SetWindowPos(windowPos);
	}
}

void GenerateWindow::Update(double dTime) {

}
