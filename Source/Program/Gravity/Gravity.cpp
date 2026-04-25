// ◦ Xyz ◦

#include "Gravity.h"
#include <GuiWindow/GuiWindows.h>
#include <Files/FileManager.h>
#include <Callback/VirtualKey.h>
#include <CudaWrapper.h>
#include <CudaExample.h>
#include <Draw/Camera.h>
#include <Draw/Draw.h>
#include "Spaces/SpaceManager.h"
#include "Windows/TopPanel.h"
#include "Windows/DebugWindow.h"
#include "Windows/GenerateWindow.h"
#include "Windows/AlgorithmWindow.h"
#include "Windows/BottomPanel.h"
#include "Windows/RenderWindow.h"
#include "Windows/GuiStyle.h"
#include "Render/GravityRender.h"
#include <Common/JsonHelper.h>
#include "Tests/Test.h"
#include <Logs.h>

Engine::Program::Uptr instanceProgram = Engine::Program::MakeProgram<Gravity>();

bool Gravity::InitFileManagers(std::string_view params)
{
	Engine::FileManager::Make("write");
	Engine::FileManager::Make("base", "../../Source/Resources/Files");
	return true;
}

bool Gravity::Init(std::string_view params)
{
	//Tests::SharedPtr();

	InitCallback();
	InitWidows();

	SpaceManager::Load();
	GravityRender::Init();

	/*{
		Cuda::ValueWrapper<float> valueW(10000);
		Cuda::VectorWrapper<Cuda::Body> dataW = std::vector<Cuda::Body>{ {1.f, {1.f, 2.f, 3.f}, {1.f, 2.f, 3.f}}, {2.f, {10.f, 20.f, 30.f}, {11.f, 22.f, 33.f}} };

		LOG("AFTER: valueW: '{}' dataW: {}", valueW, dataW);

		Cuda::CudaWrapper::Calculate(dataW, valueW);

		std::vector<BodyData> bodies = BodyData::Copy(dataW.GetValue());

		LOG("BEFORE: valueW: '{}' dataW: {} : bodies: {}", valueW, dataW, bodies);
	}*/

	return true;
}

void Gravity::OnClose()
{
	if (auto* data = Engine::Settings::Instance().JsonData("openWindows", true)) {
		data->clear();
		(*data) = Json::array();
		
		for (const auto& [name, _] : Engine::GuiWindows::GetWindows())  {
			data->emplace_back(name);
		}
	}

	SpaceManager::Save();
	//Engine::Camera::GetLink().Save();
	Engine::Camera::Get()->Save();
}

void Gravity::Update(double deltaTime)
{
	SpaceManager::Update(deltaTime);
	GravityRender::Update(deltaTime);
}

void Gravity::OnResize()
{
	Engine::Draw::Viewport();
	Engine::Camera::GetLink().Resize();
}

void Gravity::Draw()
{
	GravityRender::Render();
}

void Gravity::InitCallback()
{
	using namespace Engine;

	Callback::Add(Callback::Type::PRESS_KEY, [this](const Callback::EventData& data) {
		if (data.key == VirtualKey::ESCAPE) {
			Core::Close();
		}

		if (data.key == VirtualKey::F1) {
			Engine::GuiWindows::SwitchVisibleWindow<Windows::TopPanel>();
			Engine::GuiWindows::SwitchVisibleWindow<Windows::DebugWindow>();
			Engine::GuiWindows::SwitchVisibleWindow<Windows::GenerateWindow>();
			Engine::GuiWindows::SwitchVisibleWindow<Windows::AlgorithmWindow>();
			Engine::GuiWindows::SwitchVisibleWindow<Windows::BottomPanel>();
			Engine::GuiWindows::SwitchVisibleWindow<Windows::RenderWindow>();
		}
		if (data.key == VirtualKey::F5) {
			Engine::GuiWindows::SwitchVisibleWindow<Windows::TopPanel>();
		}
		if (data.key == VirtualKey::F6) {
			Engine::GuiWindows::SwitchVisibleWindow<Windows::DebugWindow>();
		}
		if (data.key == VirtualKey::F7) {
			Engine::GuiWindows::SwitchVisibleWindow<Windows::GenerateWindow>();
		}
		if (data.key == VirtualKey::F8) {
			Engine::GuiWindows::SwitchVisibleWindow<Windows::AlgorithmWindow>();
		}
		if (data.key == VirtualKey::F9) {
			Engine::GuiWindows::SwitchVisibleWindow<Windows::BottomPanel>();
		}
		if (data.key == VirtualKey::F10) {
			Engine::GuiWindows::SwitchVisibleWindow<Windows::RenderWindow>();
		}

		if (data.key == 'P') {
			static int countOfIteration = 0;
			
			if (SpaceManager::countOfIteration != 0) {
				countOfIteration = SpaceManager::countOfIteration;
				SpaceManager::countOfIteration = 0;
			}
			else if (countOfIteration != 0) {
				SpaceManager::countOfIteration = countOfIteration;
			}
			else {
				SpaceManager::countOfIteration = 1;
			}
		}

		if (Callback::KeyPressed(VirtualKey::CONTROL)) {
			if (data.key == VirtualKey::VK_1) {
				GravityRender::typeDraw.sprite = !GravityRender::typeDraw.sprite;
			}
			if (data.key == VirtualKey::VK_2) {
				GravityRender::typeDraw.point = !GravityRender::typeDraw.point;
			}
			if (data.key == VirtualKey::VK_3) {
				GravityRender::typeDraw.model = !GravityRender::typeDraw.model;
			}
			if (data.key == VirtualKey::VK_4) {
				GravityRender::typeDraw.spriteShader = !GravityRender::typeDraw.spriteShader;
			}
		}
	});
}

template <typename Twindow>
void OpenWindow(const std::string& nameWindow)
{
	if (nameWindow == Engine::ExtractClassName(typeid(Twindow).name())) {
		Engine::GuiWindows::SwitchVisibleWindow<Twindow>();
	}
}

void Gravity::InitWidows()
{
	Windows::LoadGuiStyle();

	if (const auto* data = Engine::Settings::Instance().JsonData("openWindows")) {
		if (data->is_array()) {
			for (const auto& element : *data) {
				if (element.is_string()) {
					const std::string windowName = element.get<std::string>();

					OpenWindow<Windows::TopPanel>(windowName);
					OpenWindow<Windows::DebugWindow>(windowName);
					OpenWindow<Windows::GenerateWindow>(windowName);
					OpenWindow<Windows::AlgorithmWindow>(windowName);
					OpenWindow<Windows::BottomPanel>(windowName);
					OpenWindow<Windows::RenderWindow>(windowName);
				}
			}
		}
	}
}
