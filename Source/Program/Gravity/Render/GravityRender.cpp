// ◦ Xyz ◦

#include "GravityRender.h"
#include <Files/Settings.h>
#include <Draw/Camera.h>
#include <Draw/Draw.h>
#include <Draw/DrawBuffer.h>
#include <Object/Shape.h>
#include <Object/Texture.h>
#include "../Shaders/GravityShader.h"
#include "../Cameras/GravityCameras.h"
#include "../Spaces/SpaceManager.h"


void GravityRender::Init()
{
	Engine::Draw::SetClearColor(0.1f, 0.2f, 0.3f);
	//Engine::Draw::SetClearColor(0.03f, 0.06f, 0.09f, 1.f);
	shaders::InitShaders();
	cameras::MakeCameras();
	prevPointBuffer.Init(Engine::ScreenParams::Width(), Engine::ScreenParams::Height());
	pointBuffer.Init(Engine::ScreenParams::Width(), Engine::ScreenParams::Height());
	frameBuffer.Init(Engine::ScreenParams::Width(), Engine::ScreenParams::Height());
}

void GravityRender::Update(double deltaTime)
{
	static float timerOffset = 0.f;

	if (deltaAlphaTime > 1.f) {
		deltaAlphaTime = 0.f;
		alpha = 0.002f;
	}
	else {
		deltaAlphaTime += timerOffset;
		alpha = 0.f;
	}
}

void GravityRender::Render()
{
	using namespace Engine;

	PrepareRender();
	RenderPoints();
	RenderSprite();
}

void GravityRender::PrepareRender()
{
	using namespace Engine;

	Draw::ClearColor();
	const glm::vec3 camPos = Camera::GetLink().Pos();
	SpaceManager::Current().Bodies(_renderBodies);

	std::sort(_renderBodies.begin(), _renderBodies.end(), [&camPos](const auto& leftBody, const auto& rightBody) {
		const float leftDist = glm::distance(camPos, leftBody.pos);
		const float rightDist = glm::distance(camPos, rightBody.pos);
		return leftDist > rightDist;
		});
}

void GravityRender::RenderPoints()
{
	using namespace Engine;

	if (_renderBodies.empty()) {
		return;
	}

	/*{
		std::vector<float> points;
		points.reserve(_renderBodies.size());

		for (const auto& body : _renderBodies) {
			points.emplace_back(body.pos.x);
			points.emplace_back(body.pos.y);
			points.emplace_back(body.pos.z);
		}

		std::array<float, 4> color = { 1.f, 1.f, 1.f, 1.0f };
		const auto& shader = shaders::LineShaderSingle::Instance();
		shader.UseProgram();
		shader.SetColor(color.data());

		pointBuffer.PushRender(true);
		Draw::ClearColor(0.f, 0.f, 0.f, 0.f);
		Draw::SetPointSize(2.f);
		Draw::RenderPoints(points.data(), points.size() / 3);
		pointBuffer.PopRender();
	}

	frameBuffer.PushRender();
	{
		const auto& shader = shaders::SimpleAlphaShaderSingle::Instance();
		shader.UseProgram();
		shader.SetAlpha(alpha);

		Draw::ClearColor(0.f, 0.f, 0.f, 1.f);
		Draw::BindTexture(prevPointBuffer.TextureId());
		Draw::DrawToScreen();
	}

	{
		const auto& shader = shaders::SimpleShaderSingle::Instance();
		shader.UseProgram();

		Draw::BindTexture(pointBuffer.TextureId());
		Draw::DrawToScreen();
	}
	frameBuffer.PopRender();

	{
		const auto& shader = shaders::SimpleShaderSingle::Instance();
		shader.UseProgram();

		Draw::ClearColor(0.1f, 0.2f, 0.3f, 1.f);
		Draw::BindTexture(frameBuffer.TextureId());
		Draw::DrawToScreen();
	}

	prevPointBuffer.Swap(frameBuffer);*/

	static bool _bbb_ = false;
	if (!_bbb_) {
		DrawBuffer::InitPostDraw();
		_bbb_ = true;
	}
	else {
		auto fun = []() {
			std::vector<float> points;
			points.reserve(_renderBodies.size());

			for (const auto& body : _renderBodies) {
				points.emplace_back(body.pos.x);
				points.emplace_back(body.pos.y);
				points.emplace_back(body.pos.z);
			}

			std::array<float, 4> color = { 1.f, 1.f, 1.f, 1.0f };
			const auto& shader = shaders::LineShaderSingle::Instance();
			shader.UseProgram();
			shader.SetColor(color.data());

			Draw::SetPointSize(2.f);
			Draw::RenderPoints(points.data(), points.size() / 3);
		};
		
		DrawBuffer::PostDraw(fun);
	}
}

void GravityRender::ClearPointBuffer()
{
	pointBuffer.Clear();
}

void GravityRender::RenderSprite()
{
	using namespace Engine;

	if (_renderBodies.empty()) {
		return;
	}

	//Engine::Draw::SetClearColor(0.1f, 0.2f, 0.3f);

	if (typeDraw.sprite) {
		auto& shader = shaders::BaseShaderSingle::Instance();
		shader.UseProgram();

		Draw::DepthTest(true);
		Draw::BindTexture(Texture::GetRef("Star.png").Id());

		const glm::vec3 camPos = Camera::GetLink().Pos();
		float farDist= glm::distance(camPos, _renderBodies.front().pos);
		float nearDist = glm::distance(camPos, _renderBodies.back().pos);
		float spaceDist = farDist - nearDist;
		std::array<float, 4> color = { 1.f, 1.f, 1.f, 1.f };

		for (const auto& body : _renderBodies) {
			glm::vec3 to = glm::normalize(Engine::Camera::GetLink().Pos() - body.pos);
			glm::vec3 from(0.f, 0.f, 1.f);
			glm::vec3 axis = glm::normalize(glm::cross(from, to));

			float dot = glm::dot(from, to);
			float angle = acos(dot);

			glm::mat4 mat(1.f);
			const float scale = body.diameter * scaleBody;

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

	if (typeDraw.spriteShader) {
		auto& shader = shaders::ForwardShaderSingle::Instance();
		shader.UseProgram();

		//Draw::BindTexture(Texture::GetRef("Star.png").Id());
		Draw::BindTexture(Texture::GetRef("Rgba64.png").Id());

		for (auto& object : _renderBodies) {
			shader.SetModelPos(object.pos);

			//Draw::Render(SHAPES["Sprite", true, true].mesh);
			Draw::Render(SHAPES["RgbBox", true, true].mesh);
		}
	}

	if (typeDraw.model) {
		auto& shader = shaders::BaseShaderSingle::Instance();
		shader.UseProgram();
		Draw::BindTexture(Texture::GetRef("orange_star.jpg").Id());

		for (auto& object : _renderBodies) {
			glm::mat4x4 mat = glm::translate(glm::mat4x4(1.f), object.pos);
			const float scale = object.diameter;
			mat = glm::scale(mat, glm::vec3(scale));
			shader.SetModelMatrix(mat);
			Draw::Render(SHAPES["Sphere", true, true].mesh);
		}
	}
}
