// ◦ Xyz ◦

#include "Gravity.h"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <mystd_memory.h>
#include <GuiWindow/GuiWindows.h>
#include <Examples/TestClass.h>
#include <Temp/Test.h>
#include <Files/FileManager.h>
#include <Files/Settings.h>
#include <Callback/VirtualKey.h>
#include <Draw/Camera.h>
#include <Draw/Draw.h>
#include <Object/Shape.h>
#include <Object/Texture.h>
#include "Shaders/GravityShader.h"
#include "Cameras/GravityCameras.h"
#include "Spaces/SpaceManager.h"
#include "Windows/TopPanel.h"
#include "Windows/DebugWindow.h"
#include "Windows/GenerateWindow.h"
#include "Windows/AlgorithmWindow.h"
#include "../Temp/LogSpecification.h"
#include "../Temp/LogMyStlSpecification.h"
#include "../Temp/LogStlSpecification.h"
#include <Log.h>

Engine::Program::Uptr instanceProgram = Engine::Program::MakeProgram<Gravity>();

using namespace mystd::Examples;

struct TypeDraw
{
	bool model = true;
	bool sprite = false;
	bool spriteShader = false;
	bool point = false;
	
} typeDraw;

bool Gravity::Init(std::string_view params)
{
	LOG("Gravity::Init");
	TestFun();

	InitFileManagers();
	InitCallback();
	InitWidows();
    InitDraw();
	SpaceManager::Load();

	LOG("Gravity::Inited");
	return true;
}

void Gravity::OnClose() {
	LOG("Gravity::OnClose");
	
	SpaceManager::Save();
	Engine::Camera::GetLink().Save();

	LOG("Gravity::OnClosed");
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
			Engine::GuiWindows::SwitchVisibleWindow<Windows::TopPanel>();
		}
		if (data.key == VirtualKey::F2) {
			Engine::GuiWindows::SwitchVisibleWindow<Windows::DebugWindow>();
		}
		if (data.key == VirtualKey::F3) {
			Engine::GuiWindows::SwitchVisibleWindow<Windows::GenerateWindow>();
		}
		if (data.key == VirtualKey::F4) {
			Engine::GuiWindows::SwitchVisibleWindow<Windows::AlgorithmWindow>();
		}

		if (data.key == 'P') {
			SpaceManager::Current().SwitchPause();
		}

		if (Callback::KeyPressed(VirtualKey::CONTROL)) {
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
		}
	});
}

void Gravity::InitWidows()
{
	Engine::GuiWindows::SwitchVisibleWindow<Windows::TopPanel>();
	Engine::GuiWindows::SwitchVisibleWindow<Windows::DebugWindow>();
	Engine::GuiWindows::SwitchVisibleWindow<Windows::GenerateWindow>();
	Engine::GuiWindows::SwitchVisibleWindow<Windows::AlgorithmWindow>();
}

void Gravity::InitDraw()
{
	Engine::Draw::SetClearColor(0.1f, 0.2f, 0.3f);
	shaders::InitShaders();
	cameras::MakeCameras();
}

void Gravity::TestDraw()
{
	using namespace Engine;

	Space& space = SpaceManager::Current();
	Draw::ClearColor();

	if (typeDraw.model) {
		auto& shader = shaders::BaseShaderSingle::Instance();
		shader.UseProgram();
		Draw::BindTexture(Texture::GetRef("orange_star.jpg").Id());

		for (auto& object : space.Objects()) {
			//shader.SetModelPos(object.pos);
			glm::mat4x4 mat = glm::translate(glm::mat4x4(1.f), object.pos);
			const float scale = object.Scale();
			mat = glm::scale(mat, glm::vec3(scale));
			shader.SetModelMatrix(mat);
			Draw::Render(SHAPES["Sphere", true, true].mesh);
		}
	}

	if (typeDraw.sprite) {
		auto& shader = shaders::BaseShaderSingle::Instance();
		shader.UseProgram();

		Draw::DepthTest(false);
		Draw::BindTexture(Texture::GetRef("Star.png").Id());
		//Draw::BindTexture(Texture::GetRef("Cloud.png").Id());

		for (auto& object : space.Objects()) {
			glm::vec3 to = glm::normalize(Engine::Camera::GetLink().Pos() - object.pos);
			glm::vec3 from(0.f, 0.f, 1.f);
			glm::vec3 axis = glm::normalize(glm::cross(from, to));

			float dot = glm::dot(from, to);
			float angle = acos(dot);

			glm::mat4 mat(1.f);
			const float scale = object.Scale() * 10.f;
			
			mat = glm::translate(mat, object.pos);

			if (glm::length(axis)) {
				mat = glm::rotate(mat, angle, axis);
			}
			mat = glm::scale(mat, glm::vec3(scale));
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
