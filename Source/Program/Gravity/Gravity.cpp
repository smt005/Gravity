// ◦ Xyz ◦

#include "Gravity.h"
#include <FileManager/FileManager.h>
#include <Callback/VirtualKey.h>
#include <Draw/Camera.h>
#include <Draw/Mesh.h>
#include <Draw/Draw.h>
#include "GravityShader.h"
#include "GravityCameras.h"
#include "GravitySpace.h"
#include "Object.h"
#include <glm/gtc/type_ptr.hpp>

#include "../Temp/LogSpecification.h"
#include "../Temp/LogStlSpecification.h"
#include <Log.h>

Engine::Program::Uptr instanceProgram = Engine::Program::MakeProgram<Gravity>();
Engine::Mesh mesh;

bool Gravity::Init(std::string_view params)
{
	InitFileManagers();
	InitCallback();
    InitDraw();
	return true;
}

void Gravity::OnClose() {}

void Gravity::Update() {
	if (Space* space = dynamic_cast<Space*>(Space::MakeItem<Space>("First").get())) {
		space->Update();
	}
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
}

void Gravity::InitDraw()
{
	using namespace Engine;

	try {
		Draw::Init();
		Draw::SetClearColor(0.3f, 0.6f, 0.9f);

		shaders::InitShaders();
		cameras::MakeCameras();

		mesh.Load(true);
	}
	catch (const std::exception& exc) {
		LOG("EXCEPTION: {}", exc);
		__debugbreak();
	}
	catch (...) {
		__debugbreak();
	}

	if (Space* space = dynamic_cast<Space*>(Space::MakeItem<Space>("First").get())) {
		space->Generate(200, 10);
	}
}

void Gravity::TestDraw()
{
	using namespace Engine;

	Draw::ClearColor();
	auto& shader = shaders::ColorShader::Instance();
	shader.UseProgram();

	if (Space* space = dynamic_cast<Space*>(Space::MakeItem<Space>("First").get())) {
		for (auto& object : space->Objects()) {
			shader.SetModelPos(glm::value_ptr(object.pos));
			Draw::Render(mesh);
		}
	}

}
