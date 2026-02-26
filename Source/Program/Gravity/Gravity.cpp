// ◦ Xyz ◦

#include "Gravity.h"
#include <FileManager/FileManager.h>
#include <Callback/VirtualKey.h>
#include <Draw/Camera.h>
#include <Draw/Draw.h>
#include "GravityShader.h"
#include "GravityCameras.h"
#include "GravitySpace.h"
#include <glm/gtc/type_ptr.hpp>

#include "../Temp/LogSpecification.h"
#include "../Temp/LogStlSpecification.h"
#include <Log.h>

Engine::Program::Uptr instanceProgram = Engine::Program::MakeProgram<Gravity>();

bool Gravity::Init(std::string_view params)
{
	InitFileManagers();
	InitCallback();
    InitDraw();
	return true;
}

void Gravity::OnClose() {}

void Gravity::Update() {
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
	LOG("FileManager: {}", Engine::FileManager::Get("base"));
}

void Gravity::Draw() {
    TestDraw();
}

void Gravity::InitCallback()
{
	using namespace Engine;

	Callback::Add(Callback::Type::PRESS_KEY, [](const Callback::EventData& data) {
		if (data.key == VirtualKey::ESCAPE) {
			exit(0);
		}
	});

	Callback::Add(Callback::Type::PRESS_KEY, [](const Callback::EventData& data) {
		if (Callback::KeyPressed(VirtualKey::CONTROL)) {
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
		}
		});

	/*Callback::Add(Callback::Type::PINCH_KEY, [mat = glm::mat4x4(1.f)](const Callback::EventData&) mutable {
		const float speed = Callback::KeyPressed(VirtualKey::SHIFT) ? 0.025f : 0.005f;

		if (Callback::KeyPressed('W')) {
			mat[3][1] += speed;
			shaders::ColorShader::Instance().SetModelMatrix(glm::value_ptr(mat));
		}
		if (Callback::KeyPressed('S')) {
			mat[3][1] -= speed;
			shaders::ColorShader::Instance().SetModelMatrix(glm::value_ptr(mat));
		}

		if (Callback::KeyPressed('A')) {
			mat[3][0] -= speed;
			shaders::ColorShader::Instance().SetModelMatrix(glm::value_ptr(mat));
		}
		if (Callback::KeyPressed('D')) {
			mat[3][0] += speed;
			shaders::ColorShader::Instance().SetModelMatrix(glm::value_ptr(mat));
		}

		if (Callback::KeyPressed('R')) {
			mat[3][2] -= speed;
			shaders::ColorShader::Instance().SetModelMatrix(glm::value_ptr(mat));
		}
		if (Callback::KeyPressed('F')) {
			mat[3][2] += speed;
			shaders::ColorShader::Instance().SetModelMatrix(glm::value_ptr(mat));
		}

		if (Callback::KeyPressed('Q')) {
			mat = glm::rotate(mat, speed, {0.f, 0.f, 1.f});
			shaders::ColorShader::Instance().SetModelMatrix(glm::value_ptr(mat));
		}
		if (Callback::KeyPressed('E')) {
			mat = glm::rotate(mat, -speed, { 0.f, 0.f, 1.f });
			shaders::ColorShader::Instance().SetModelMatrix(glm::value_ptr(mat));
		}
		});*/
}

void Gravity::InitDraw()
{
	using namespace Engine;

	try {
		Draw::Init();
		Draw::SetClearColor(0.3f, 0.6f, 0.9f);

		shaders::InitShaders();
		cameras::MakeCameras();
	}
	catch (const std::exception& exc) {
		LOG("EXCEPTION: {}", exc);
		__debugbreak();
	}
	catch (...) {
		__debugbreak();
	}

	//Space::MakeItem<Space>("First");
}

void Gravity::TestDraw()
{
	using namespace Engine;

	Draw::ClearColor();
	shaders::ColorShader::Instance().UseProgram();


	Draw::Render();
}
