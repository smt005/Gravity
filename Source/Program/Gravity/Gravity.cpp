// ◦ Xyz ◦

#include "Gravity.h"
#include <GuiWindow/GuiWindows.h>
#include <Temp/Test.h>
#include <Files/FileManager.h>
#include <Callback/VirtualKey.h>
#include <Draw/Camera.h>
#include <Draw/Draw.h>
#include "Spaces/SpaceManager.h"
#include "Windows/TopPanel.h"
#include "Windows/DebugWindow.h"
#include "Windows/GenerateWindow.h"
#include "Windows/AlgorithmWindow.h"
#include "Windows/BottomPanel.h"
#include "Render/GravityRender.h"
#include "../Temp/LogSpecification.h"
#include "../Temp/LogMyStlSpecification.h"
#include "../Temp/LogStlSpecification.h"
#include <Log.h>

Engine::Program::Uptr instanceProgram = Engine::Program::MakeProgram<Gravity>();

bool Gravity::Init(std::string_view params)
{
	TestFun();

	InitFileManagers();
	InitCallback();
	InitWidows();

	SpaceManager::Load();
	GravityRender::Init();
	
	return true;
}

void Gravity::OnClose() {
	SpaceManager::Save();
	Engine::Camera::GetLink().Save();
}

void Gravity::Update(double deltaTime) {
	SpaceManager::Update(deltaTime);
}

void Gravity::OnResize()
{
	Engine::Draw::Viewport();
	Engine::Camera::GetLink().Resize();
}

void Gravity::InitFileManagers()
{
	Engine::FileManager::Make("write");
	Engine::FileManager::Make("base", "../../Source/Resources/Files");
}

void Gravity::Draw() {
	GravityRender::Render();
}

void Gravity::InitCallback()
{
	using namespace Engine;

	Callback::Add(Callback::Type::PRESS_KEY, [this](const Callback::EventData& data) {
		if (data.key == VirtualKey::ESCAPE) {
			Core::Close();
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
				GravityRender::typeDraw.model = !GravityRender::typeDraw.model;
			}
			if (data.key == VirtualKey::VK_2) {
				GravityRender::typeDraw.sprite = !GravityRender::typeDraw.sprite;
			}
			if (data.key == VirtualKey::VK_3) {
				GravityRender::typeDraw.spriteShader = !GravityRender::typeDraw.spriteShader;
			}
			if (data.key == VirtualKey::VK_4) {
				GravityRender::typeDraw.point = !GravityRender::typeDraw.point;
			}
		}
	});
}

void Gravity::InitWidows()
{
	Engine::GuiWindows::SwitchVisibleWindow<Windows::TopPanel>();
	Engine::GuiWindows::SwitchVisibleWindow<Windows::DebugWindow>();
	Engine::GuiWindows::SwitchVisibleWindow<Windows::GenerateWindow>();
	Engine::GuiWindows::SwitchVisibleWindow<Windows::AlgorithmWindow>();
	Engine::GuiWindows::SwitchVisibleWindow<Windows::BottomPanel>();
}
