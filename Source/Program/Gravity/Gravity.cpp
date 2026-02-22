// ◦ Xyz ◦

#include "Gravity.h"
#include <FileManager/FileManager.h>
#include <Callback/VirtualKey.h>
#include <ImGuiWindow/ImGuiWindow.h>
#include <imgui.h>

#include "../Temp/LogSpecification.h"
#include <Log.h>

Engine::Program::Uptr instanceProgram = Engine::Program::MakeProgram<Gravity>();
std::string windowName = "Test";

bool Gravity::Init(std::string_view params)
{
	InitFileManagers();
	InitCallback();
	TestWindows();
	return true;
}

void Gravity::Update() {}
void Gravity::Draw() {}
void Gravity::OnResize() {}
void Gravity::OnClose() {}

void Gravity::InitFileManagers()
{
	Engine::FileManager::Make("write");
	Engine::FileManager::Make("base", "../../Source/Resources/Files");
}

void Gravity::InitCallback()
{
	using namespace Engine;

	Callback::Add(Callback::Type::PRESS_KEY, [this](const Callback::EventData& data) {
		if (data.key == VirtualKey::ESCAPE) {
			exit(0);
		}
	});

	Callback::Add(Callback::Type::PRESS_KEY, [this](const Callback::EventData& data) {
		if (Callback::KeyPressed(VirtualKey::CONTROL)) {
			if (data.key == VirtualKey::F1) {
				TestWindows();
			}

			if (data.key == 'F' && ImGuiWindow::ExistWindow(windowName)) {
				ImGuiWindow::Get(windowName)->SetFullScreen(!ImGuiWindow::Get(windowName)->IsFullScreen());
			}
		}
		});
}

void Gravity::TestWindows()
{
	using namespace Engine;

	struct TestWindow : public ImGuiWindow {
		TestWindow() : Engine::ImGuiWindow() {}
		TestWindow(const std::string& name) : Engine::ImGuiWindow(name) {}
		void Render() override {
			ImGui::Text("name: %s", GetName().c_str());

			ImGui::Dummy(ImVec2(0.f, 0.f));
			ImGui::Text("title: %s", GetTitle().c_str());

			ImGui::Dummy(ImVec2(0.f, 0.f));
			ImGui::Separator();
			std::string text = TO_STRING("RENDER {} value: {}", GetName(), value);
			ImGui::Text("%s", text.c_str());

			if (ImGui::Button("Close", { 100.f, 30.f })) {
				Close();
			}
		}
		void Update(double dTime) override {
			++value;
		}
		void OnClose() override {
			LOG("CLOSE value: {}", value);
		}
		int value = 0;
	};

	if (ImGuiWindow::ExistWindow(windowName)) {
		ImGuiWindow::CloseWindow(windowName);
	}
	else {
		windowName = ImGuiWindow::MakeWindow<TestWindow>(windowName)->GetName();
		//windowName = ImGuiWindow::MakeWindow<TestWindow>()->GetName();
	}
}
