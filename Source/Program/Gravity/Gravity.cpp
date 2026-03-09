// ◦ Xyz ◦

#include "Gravity.h"
#include <FileManager/FileManager.h>
#include <Callback/VirtualKey.h>
#include <Draw/Camera.h>
#include <Draw/Draw.h>
#include <Object/Shape.h>
#include <Object/Texture.h>
#include "GravityShader.h"
#include "GravityCameras.h"
#include "Spaces/GravitySpace.h"
#include <mystd_memory.h>
#include "Windows/TopPanel.h"
#include "Windows/DebugWindow.h"
#include <Examples/TestClass.h>
#include "../Temp/LogSpecification.h"
#include "../Temp/LogMyStlSpecification.h"
#include "../Temp/LogStlSpecification.h"
#include <Log.h>
#include <glm/gtc/type_ptr.hpp>

Engine::Program::Uptr instanceProgram = Engine::Program::MakeProgram<Gravity>();

using namespace mystd::Examples;
mystd::shared_ptr<TestClass> testShader;

bool Gravity::Init(std::string_view params)
{
	InitFileManagers();
	InitCallback();
	InitWidows();
    InitDraw();
	TestSimpleShared();
	return true;
}

void Gravity::OnClose() {}

void Gravity::Update() {
	Space::Instance().Update();
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
    TestDraw();
}

void Gravity::InitCallback()
{
	using namespace Engine;

	Callback::Add(Callback::Type::PRESS_KEY, [](const Callback::EventData& data) {
		if (data.key == VirtualKey::ESCAPE) {
			Core::Close();
		}

		if (data.key == VirtualKey::F1) {
			Windows::TopPanel::SwitchVisibleWindow();
		}
		if (data.key == VirtualKey::F2) {
			Windows::DebugWindow::SwitchVisibleWindow();
		}
	});
}

void Gravity::InitWidows()
{
	Windows::TopPanel::OpenWindow();
	Windows::DebugWindow::OpenWindow();
}

void Gravity::InitDraw()
{
	using namespace Engine;

	try {
		Draw::SetClearColor(0.1f, 0.2f, 0.3f);
		shaders::InitShaders();
		cameras::MakeCameras();

#ifdef _DEBUG
		Space::Instance().Generate(250, 100);
#else
		Space::Instance().Generate(1000, 100);
#endif

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
	auto& shader = shaders::BaseShader::Instance();
	shader.UseProgram();

	Draw::BindTexture(Texture::GetRef("orange_star.jpg").Id());

	Space& space = Space::Instance();
	for (auto& object : space.Objects()) {
		shader.SetModelPos(glm::value_ptr(object.pos));
		//Draw::Render(SHAPES["Star", true, true].mesh);
		Draw::Render(SHAPES["Sphere", true, true].mesh);
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
