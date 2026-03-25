// ◦ Xyz ◦

#include "GravityRender.h"
#include <Files/Settings.h>
#include <Draw/Camera.h>
#include <Draw/Draw.h>
#include <Object/Shape.h>
#include <Object/Texture.h>
#include "../Shaders/GravityShader.h"
#include "../Cameras/GravityCameras.h"
#include "../Spaces/Object.h"
#include "../Spaces/SpaceManager.h"

void GravityRender::Init()
{
	Engine::Draw::SetClearColor(0.1f, 0.2f, 0.3f);
	shaders::InitShaders();
	cameras::MakeCameras();
}

void GravityRender::Render()
{
	using namespace Engine;

	Draw::ClearColor();

	if (typeDraw.model) {
		Space& space = SpaceManager::Current();
		auto& shader = shaders::BaseShaderSingle::Instance();
		shader.UseProgram();
		Draw::BindTexture(Texture::GetRef("orange_star.jpg").Id());

		for (auto& object : space.Objects()) {
			glm::mat4x4 mat = glm::translate(glm::mat4x4(1.f), object.pos);
			const float scale = object.Diameter();
			mat = glm::scale(mat, glm::vec3(scale));
			shader.SetModelMatrix(mat);
			Draw::Render(SHAPES["Sphere", true, true].mesh);
		}
	}

	if (typeDraw.sprite) {
		Space& space = SpaceManager::Current();
		auto& shader = shaders::BaseShaderSingle::Instance();
		shader.UseProgram();

		Draw::DepthTest(false);
		Draw::BindTexture(Texture::GetRef("Star.png").Id());

		for (auto& object : space.Objects()) {
			glm::vec3 to = glm::normalize(Engine::Camera::GetLink().Pos() - object.pos);
			glm::vec3 from(0.f, 0.f, 1.f);
			glm::vec3 axis = glm::normalize(glm::cross(from, to));

			float dot = glm::dot(from, to);
			float angle = acos(dot);

			glm::mat4 mat(1.f);
			static float scaleStar = Engine::GetJsonValue("scale", Engine::Settings::Instance().JsonData("test"), 1.f);
			const float scale = object.Diameter() * scaleStar;

			mat = glm::translate(mat, object.pos);

			if (glm::length(axis)) {
				mat = glm::rotate(mat, angle, axis);
			}
			mat = glm::scale(mat, glm::vec3(scale));
			shader.SetModelMatrix(mat);

			Draw::Render(SHAPES["Sprite", true, true].mesh);
		}
	}

	if (typeDraw.spriteShader) {
		Space& space = SpaceManager::Current();
		auto& shader = shaders::ForwardShaderSingle::Instance();
		shader.UseProgram();

		//Draw::BindTexture(Texture::GetRef("Star.png").Id());
		Draw::BindTexture(Texture::GetRef("Rgba64.png").Id());

		for (auto& object : space.Objects()) {
			shader.SetModelPos(object.pos);

			//Draw::Render(SHAPES["Sprite", true, true].mesh);
			Draw::Render(SHAPES["RgbBox", true, true].mesh);
		}
	}

	if (typeDraw.point) {
		Draw::SetPointSize(1.f);
		const auto& shader = shaders::LineShaderSingle::Instance();
		shader.UseProgram();

		std::vector<float> points;
		GetBodyPositions(points);

		Draw::RenderPoints(points.data(), points.size() / 3);
	}
}

void GravityRender::GetBodyPositions(std::vector<float>& data)
{
	std::vector<glm::vec3> bodyPoses;
	SpaceManager::GetBodyPositions(bodyPoses);
	std::vector<float> points;
	points.reserve(bodyPoses.size() * 3);

	for (auto& pos : bodyPoses) {
		points.emplace_back(pos.x);
		points.emplace_back(pos.y);
		points.emplace_back(pos.z);
	}

	std::swap(data, points);
}

void GravityRender::GetBodyPositions(std::vector<glm::vec3>& vec3Data)
{
	SpaceManager::GetBodyPositions(vec3Data);
}
