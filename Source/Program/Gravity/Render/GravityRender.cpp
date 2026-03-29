// ◦ Xyz ◦

#include "GravityRender.h"
#include <Files/Settings.h>
#include <Draw/Camera.h>
#include <Draw/Draw.h>
#include <Object/Shape.h>
#include <Object/Texture.h>
#include "../Shaders/GravityShader.h"
#include "../Cameras/GravityCameras.h"
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
	const glm::vec3 camPos = Camera::GetLink().Pos();

	// TODO
	std::sort(SpaceManager::bodies.begin(), SpaceManager::bodies.end(), [&camPos](const auto& leftBody, const auto& rightBody) {
		const float leftDist = glm::distance(camPos, leftBody.pos);
		const float rightDist = glm::distance(camPos, rightBody.pos);
		return leftDist > rightDist;
	});

	// TODO:
	if (SpaceManager::bodies.empty()) {
		return;
	}

	if (typeDraw.sprite) {
		
		auto& shader = shaders::BaseShaderSingle::Instance();
		shader.UseProgram();

		Draw::DepthTest(true);
		Draw::BindTexture(Texture::GetRef("Star.png").Id());

		float farDist= glm::distance(camPos, SpaceManager::bodies.front().pos);
		float nearDist = glm::distance(camPos, SpaceManager::bodies.back().pos);
		float spaceDist = farDist - nearDist;
		std::array<float, 4> color = { 1.f, 1.f, 1.f, 1.f };

		for (const auto& body : SpaceManager::bodies) {
			glm::vec3 to = glm::normalize(Engine::Camera::GetLink().Pos() - body.pos);
			glm::vec3 from(0.f, 0.f, 1.f);
			glm::vec3 axis = glm::normalize(glm::cross(from, to));

			float dot = glm::dot(from, to);
			float angle = acos(dot);

			glm::mat4 mat(1.f);
			static float scaleStar = Engine::GetJsonValue("scale", Engine::Settings::Instance().JsonData("test"), 1.f);
			const float scale = body.Diameter() * scaleStar;

			mat = glm::translate(mat, body.pos);

			if (glm::length(axis)) {
				mat = glm::rotate(mat, angle, axis);
			}
			mat = glm::scale(mat, glm::vec3(scale));
			shader.SetModelMatrix(mat);

			float distFactor = glm::distance(camPos, body.pos);
			distFactor -= nearDist;
			distFactor /= spaceDist;

			color[1] = 1.f - distFactor;
			color[2] = 1.f - distFactor;

			shader.SetColor(color.data());

			Draw::Render(SHAPES["Sprite", true, true].mesh);
		}
	}

	if (typeDraw.point) {
		Draw::SetPointSize(1.f);
		const auto& shader = shaders::LineShaderSingle::Instance();
		shader.UseProgram();

		std::vector<float> points;
		GetBodyPositions(points);

		std::array<float, 4> color = { 10.f, 0.f, 0.f, 1.f };
		shader.SetColor(color.data());

		Draw::RenderPoints(points.data(), points.size() / 3);
	}

	if (typeDraw.spriteShader) {
		auto& shader = shaders::ForwardShaderSingle::Instance();
		shader.UseProgram();

		//Draw::BindTexture(Texture::GetRef("Star.png").Id());
		Draw::BindTexture(Texture::GetRef("Rgba64.png").Id());

		for (auto& object : SpaceManager::bodies) {
			shader.SetModelPos(object.pos);

			//Draw::Render(SHAPES["Sprite", true, true].mesh);
			Draw::Render(SHAPES["RgbBox", true, true].mesh);
		}
	}

	if (typeDraw.model) {
		auto& shader = shaders::BaseShaderSingle::Instance();
		shader.UseProgram();
		Draw::BindTexture(Texture::GetRef("orange_star.jpg").Id());

		for (auto& object : SpaceManager::bodies) {
			glm::mat4x4 mat = glm::translate(glm::mat4x4(1.f), object.pos);
			const float scale = object.Diameter();
			mat = glm::scale(mat, glm::vec3(scale));
			shader.SetModelMatrix(mat);
			Draw::Render(SHAPES["Sphere", true, true].mesh);
		}
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
