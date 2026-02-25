// ◦ Xyz ◦

#include "Gravity.h"
#include <FileManager/FileManager.h>
#include <Callback/VirtualKey.h>
#include <Draw/Camera.h>
#include <Draw/Draw.h>

#include "../Temp/LogSpecification.h"
#include "../Temp/LogStlSpecification.h"
#include <Log.h>

#include "GravityShader.h"

Engine::Program::Uptr instanceProgram = Engine::Program::MakeProgram<Gravity>();

float vertices[] = {
     0.0f,  0.5f, 0.0f,  // верх
    -0.5f, -0.5f, 0.0f,  // левый низ
     0.5f, -0.5f, 0.0f   // правый низ
};

bool Gravity::Init(std::string_view params)
{
	InitFileManagers();
	InitCallback();

    InitDraw();
	return true;
}

void Gravity::OnClose() {}

void Gravity::Update() {}

void Gravity::OnResize()
{
	Engine::Draw::Viewport();
}

void Gravity::InitFileManagers()
{
	Engine::FileManager::Make("write");
	Engine::FileManager::Make("base", "../../Source/Resources/Files");
	LOG("FileManager: {}", Engine::FileManager::Get("base"));
}

void Gravity::Draw() {
    TestDraw();
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
		if (data.key == 'R') {
			float color[] = { 1.f, 0.f, 0.f, 1.f };
			shaders::ColorShader::Instance().SetColor(color);
		}
		if (data.key == 'G') {
			float color[] = { 0.f, 1.f, 0.f, 1.f };
			shaders::ColorShader::Instance().SetColor(color);
		}
		if (data.key == 'B') {
			float color[] = { 0.f, 0.f, 1.f, 1.f };
			shaders::ColorShader::Instance().SetColor(color);
		}
		});
}

void Gravity::InitDraw()
{
	using namespace Engine;

	try {
		Draw::Init();
		Draw::SetClearColor(0.3f, 0.6f, 0.9f);
	
		//auto& camera = *Camera::MakeAndSet<Camera>();
		//camera.SetPerspective(10000000.f, 1.f, 45.f);
		//camera.SetDirect({0.f, 0.f, -1.f});
		//camera.SetPos({ 0.f, 0.f, 10.f });

		shaders::InitShaders();
	}
	catch (const std::exception& exc) {
		LOG("EXCEPTION: {}", exc);
		__debugbreak();
	}
	catch (...) {
		__debugbreak();
	}
}

void Gravity::TestDraw()
{
	using namespace Engine;

	Draw::ClearColor();
	shaders::ColorShader::Instance().UseProgram();
	Draw::Render();
}
