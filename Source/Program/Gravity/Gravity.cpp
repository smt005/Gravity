// ◦ Xyz ◦

#include "Gravity.h"
#include <FileManager/FileManager.h>
#include <Callback/VirtualKey.h>
#include <Draw/Camera.h>
#include <Draw/Draw.h>
#include <Object/Shape.h>
#include <Object/Texture.h>
#include "Shaders/GravityShader.h"
#include "Cameras/GravityCameras.h"
#include "Spaces/Space.h"
#include "Spaces/MainThreadSpace.h"
#include "Spaces/SpaceManager.h"
#include <mystd_memory.h>
#include "Windows/TopPanel.h"
#include "Windows/DebugWindow.h"
#include <Examples/TestClass.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Temp/LogSpecification.h"
#include "../Temp/LogMyStlSpecification.h"
#include "../Temp/LogStlSpecification.h"
#include <Log.h>

Engine::Program::Uptr instanceProgram = Engine::Program::MakeProgram<Gravity>();

using namespace mystd::Examples;
mystd::shared_ptr<TestClass> testShader;

struct TypeDraw
{
	bool model = true;
	bool sprite = false;
	bool spriteShader = false;
	bool point = false;
	
} typeDraw;

bool Gravity::Init(std::string_view params)
{
	InitFileManagers();
	InitCallback();
	InitWidows();
    InitDraw();
	TestSimpleShared();

	LOG("Gravity::Init");
	return true;
}

void Gravity::OnClose() {
	LOG("Gravity::OnClose");
}

void Gravity::Update() {
	SpaceManager::Update();
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

	Callback::Add(Callback::Type::PRESS_KEY, [this](const Callback::EventData& data) {
		if (data.key == VirtualKey::ESCAPE) {
			Core::Close();
		}

		if (data.key == VirtualKey::F1) {
			Windows::TopPanel::SwitchVisibleWindow();
		}
		if (data.key == VirtualKey::F2) {
			Windows::DebugWindow::SwitchVisibleWindow();
		}

		if (data.key == 'P') {
			SpaceManager::Current().SwitchPause();
		}

		if (Callback::KeyPressed(VirtualKey::CONTROL)) {
			if (data.key == 'P') {
#ifdef _DEBUG
				SpaceManager::SetCurrent<Space>().Generate(20000, 300);
#else
				SpaceManager::SetCurrent<Space>().Generate(100000, 300);
#endif

			}
			if (data.key == 'O') {
#ifdef _DEBUG
				SpaceManager::SetCurrent<MainThreadSpace>().Generate(200, 100);
#else
				SpaceManager::SetCurrent<MainThreadSpace>().Generate(1000, 300);
#endif
			}
		}

		if (data.key == VirtualKey::VK_1) {
			typeDraw.model = !typeDraw.model;
		}
		if (data.key == VirtualKey::VK_2) {
			typeDraw.sprite = !typeDraw.sprite;
		}
		if (data.key == VirtualKey::VK_3) {
			typeDraw.spriteShader = !typeDraw.spriteShader;
		}
		if (data.key == VirtualKey::VK_4) {
			typeDraw.point = !typeDraw.point;
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

	Draw::SetClearColor(0.1f, 0.2f, 0.3f);
	shaders::InitShaders();
	cameras::MakeCameras();
	SpaceManager::Current().Generate(1000, 100);
}

void Gravity::TestDraw()
{
	using namespace Engine;

	Space& space = SpaceManager::Current();
	//Space space;
	Draw::ClearColor();

	if (typeDraw.model) {
		auto& shader = shaders::BaseShaderSingle::Instance();
		shader.UseProgram();
		Draw::BindTexture(Texture::GetRef("orange_star.jpg").Id());

		for (auto& object : space.Objects()) {
			shader.SetModelPos(object.pos);
			Draw::Render(SHAPES["Sphere", true, true].mesh);
		}
	}

	if (typeDraw.sprite) {
		auto& shader = shaders::BaseShaderSingle::Instance();
		shader.UseProgram();

		Draw::DepthTest(false);
		Draw::BindTexture(Texture::GetRef("Star.png").Id());
		//Draw::BindTexture(Texture::GetRef("Rgba64.png").Id());

		for (auto& object : space.Objects()) {
			glm::vec3 to = glm::normalize(Engine::Camera::GetLink().Pos() - object.pos);
			glm::vec3 from(0.f, 0.f, 1.f);
			glm::vec3 axis = glm::normalize(glm::cross(from, to));

			float dot = glm::dot(from, to);
			float angle = acos(dot);

			glm::mat4 mat(1.f);
			mat = glm::translate(mat, object.pos);

			if (glm::length(axis)) {
				mat = glm::rotate(mat, angle, axis);
			}

			shader.SetModelMatrix(mat);

			Draw::Render(SHAPES["Star5", true, true].mesh);
			//Draw::Render(SHAPES["RgbBox", true, true].mesh);
		}
	}

	if (typeDraw.spriteShader) {
		auto& shader = shaders::ForwardShaderSingle::Instance();
		shader.UseProgram();

		//Draw::BindTexture(Texture::GetRef("Star.png").Id());
		Draw::BindTexture(Texture::GetRef("Rgba64.png").Id());

		for (auto& object : space.Objects()) {
			shader.SetModelPos(object.pos);

			//Draw::Render(SHAPES["Star5", true, true].mesh);
			Draw::Render(SHAPES["RgbBox", true, true].mesh);
		}
	}

	if (typeDraw.point) {
		Draw::SetPointSize(2.f);
		const auto& shader = shaders::LineShaderSingle::Instance();
		shader.UseProgram();

		/*for (auto& object : space.Objects()) {
			Draw::RenderPoints(glm::value_ptr(object.pos), 1);
		}*/

		unsigned int countPoints = space.Objects().size();
		std::vector<float> points;
		points.reserve(countPoints * 3);

		for (auto& object : space.Objects()) {
			points.emplace_back(object.pos.x);
			points.emplace_back(object.pos.y);
			points.emplace_back(object.pos.z);
		}

		Draw::RenderPoints(points.data(), countPoints);
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
