// ◦ Xyz ◦

#include "Gravity.h"
#include <FileManager/FileManager.h>
#include <Callback/VirtualKey.h>
#include <Draw/Camera.h>
#include <Draw/Mesh.h>
#include <Draw/Draw.h>
#include <Object/Shape.h>
#include "GravityShader.h"
#include "GravityCameras.h"
#include "GravitySpace.h"
#include "Object.h"
#include <mystd_memory.h>
#include <glm/gtc/type_ptr.hpp>

#include <Examples/TestClass.h>
#include "../Temp/LogSpecification.h"
#include "../Temp/LogMyStlSpecification.h"
#include "../Temp/LogStlSpecification.h"
#include <Log.h>

Engine::Program::Uptr instanceProgram = Engine::Program::MakeProgram<Gravity>();

using namespace mystd::Examples;
mystd::shared_ptr<TestClass> testShader;

bool Gravity::Init(std::string_view params)
{
	InitFileManagers();
	InitCallback();
    InitDraw();
	TestSimpleShared();
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
		Draw::SetClearColor(0.1f, 0.2f, 0.3f);
		shaders::InitShaders();
		cameras::MakeCameras();

		{
			Shape::Ptr sp = mystd::make_shared<Shape>("qwe");
			Resource<Shape>::Ptr res = sp;
		}

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
			//Draw::Render(SHAPES["Star", true, true].mesh);
			Draw::Render(SHAPES["Sphere", true, true].mesh);
		}
	}

}

void Gravity::TestSimpleShared()
{
	/*LOG("BEGIN");
	{
		mystd::shared_ptr<TestClass> firstShader;
		mystd::shared_ptr<TestClass> secondShader = mystd::make_shared<TestClass>(111, "Test111");

		TestClass* testClassPtr = nullptr;
		{
			mystd::shared_ptr<TestClass> thirdShader(testClassPtr);
			LOG("[{}], [{}], [{}]", firstShader, *secondShader, thirdShader);
		}

		testClassPtr = new TestClass(222, "222Test");
		mystd::shared_ptr<TestClass> thirdShader(testClassPtr);
		LOG("[{}], [{}], [{}]", firstShader, secondShader, thirdShader);

		thirdShader = secondShader;
		LOG("[{}], [{}], [{}]", firstShader, secondShader, thirdShader);

		firstShader = std::move(secondShader);
		LOG("[{}], [{}], [{}]", firstShader, secondShader, thirdShader);

		mystd::shared_ptr<TestClass> forShader(firstShader);
		LOG("[{}], [{}], [{}], [{}]", firstShader, secondShader, thirdShader, forShader);

		forShader->_val = 999;
		LOG("[{}], [{}], [{}], [{}]", firstShader, secondShader, thirdShader, forShader);

		firstShader = mystd::make_shared<TestClass>(333, "333Test333");
		LOG("[{}], [{}], [{}], [{}]", firstShader, secondShader, thirdShader, forShader);

		mystd::simple_shared_ptr<TestClass> fiveShader(std::move(firstShader));
		LOG("[{}], [{}], [{}], [{}]", firstShader, secondShader, thirdShader, forShader, fiveShader);
	}
	LOG("END");*/
}
