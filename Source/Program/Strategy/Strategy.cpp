// ◦ Xyz ◦

#include "Strategy.h"
#include <GuiWindow/GuiWindows.h>
#include <Files/FileManager.h>
#include <Callback/VirtualKey.h>
#include <Object/Model.h>
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

	{
		// Sphere
		_modelPtr = MODELS.Get("NLO100", true, true);
	
		Engine::Draw::SetClearColor({0.3f, 0.6f, 0.9f, 1.f});
	}

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
		if (_modelPtr && _modelPtr->_shape) {
			auto& shader = shaders::BaseShaderSingle::Instance();
			shader.UseProgram();

			//...
			glm::mat4 mat(1.f);

			//const float scale = 10.f;
			//mat = glm::translate(mat, body.pos);
			/*if (glm::length(axis)) {
				mat = glm::rotate(mat, angle, axis);
			}*/

			//mat = glm::scale(mat, glm::vec3(scale));

			shader.SetModelMatrix(mat);
			
			//...
			Draw::ClearColor();
			Draw::ClearDepth();
			Draw::DepthTest(true);
			Draw::ActiveTexture(0);
			//Draw::BindTexture(Texture::GetRef("NLO/metallic_circle.jpg").Id());
			Draw::BindTexture(_modelPtr->_texture->Id());
			//...

			Engine::Color color({ 1.f, 1.f, 1.f, 1.f });
			shader.SetColor(color);
			Engine::Draw::Render(_modelPtr->_shape->mesh);
		}
	}
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
