// ◦ Xyz ◦

#include "Strategy.h"
#include <GuiWindow/GuiWindows.h>
#include <Files/FileManager.h>
#include <Callback/VirtualKey.h>
#include <Object/Model.h>
#include <Object/Location.h>
#include <Draw/Camera.h>
#include <Draw/Draw.h>
#include <Common/JsonHelper.h>
#include <Logs.h>
#include "../Gravity/Shaders/GravityShader.h"
#include "../Gravity/Cameras/GravityCameras.h"

Engine::Program::Uptr instanceProgram = Engine::Program::MakeProgram<Strategy>();

bool Strategy::InitFileManagers(std::string_view params)
{
	Engine::FileManager::Make("write");
	Engine::FileManager::Make("base", "../../Source/Resources/Files");
	return true;
}

bool Strategy::Init(std::string_view params)
{
	InitCallback();

	{
		Engine::Draw::SetClearColor(0.1f, 0.2f, 0.3f);
		shaders::InitShaders();
		cameras::MakeCameras();
	}

	/*{
		_modelPtr = MODELS.Get("Plane_25_100", true, true);
		Engine::Draw::SetClearColor({0.3f, 0.6f, 0.9f, 1.f});
	}*/

	return true;
}

void Strategy::OnClose()
{
	Engine::Camera::Get()->Save();
}

void Strategy::Update(double deltaTime)
{
}

void Strategy::OnResize()
{
	Engine::Draw::Viewport();
	Engine::Camera::GetLink().Resize();
}

void Strategy::Draw()
{
	using namespace Engine;

	{
		Draw::ClearColor();
		Draw::ClearDepth();
		Draw::DepthTest(true);
		Draw::ActiveTexture(0);
	}

	auto& shader = shaders::BaseShaderSingle::Instance();
	shader.UseProgram();
	
	Engine::Color color({ 1.f, 1.f, 1.f, 1.f });
	shader.SetColor(color);

	for (auto& obj : LOCATIONS["First"]._objects) {
		shader.SetModelMatrix(obj.mat);
		Draw::BindTexture(obj.model->_texture->Id());
		Engine::Draw::Render(obj.model->_shape->mesh);
	}

	for (auto& obj : LOCATIONS["First"]._gliders) {
		shader.SetModelMatrix(obj.mat);
		Draw::BindTexture(obj.model->_texture->Id());
		Engine::Draw::Render(obj.model->_shape->mesh);
	}

	/*{
		if (_modelPtr && _modelPtr->_shape) {
			glm::mat4 mat(1.f);
			shader.SetModelMatrix(mat);

			Draw::BindTexture(_modelPtr->_texture->Id());
			
			Engine::Draw::Render(_modelPtr->_shape->mesh);
		}
	}*/
}

void Strategy::InitCallback()
{
	using namespace Engine;

	Callback::Add(Callback::Type::PRESS_KEY, [this](const Callback::EventData& data) {
		if (data.key == VirtualKey::ESCAPE) {
			Core::Close();
		}
	});
}
