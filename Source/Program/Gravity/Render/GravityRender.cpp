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
#include "../Spaces/Common/SpatialGrid.h"

void GravityRender::Init()
{
	Engine::Draw::SetClearColor(0.1f, 0.2f, 0.3f);
	shaders::InitShaders();
	cameras::MakeCameras();

	pointBuffer.Create();
	traceBuffer.Create();
	resultBuffer.Create();
	tempBuffer.Create();
}

void GravityRender::Update(double deltaTime)
{
	if (traceDecayTime >= 1.f) {
		traceDecayTime = 0.f;
		traceDecay = 0.002f;
	}
	else {
		traceDecayTime += trace;
		traceDecay = 0.f;
	}
}

void GravityRender::Render()
{
	PrepareRender();
	RenderPoints();
	RenderCoordinateGrid();
	RenderSprite();
}

void GravityRender::PrepareRender()
{
	SpaceManager::Current().Bodies(_renderBodies);

	std::sort(_renderBodies.begin(), _renderBodies.end(), [&camPos = Engine::Camera::GetLink().Pos()](const auto& leftBody, const auto& rightBody) {
		const float leftDist = glm::distance(camPos, leftBody.pos);
		const float rightDist = glm::distance(camPos, rightBody.pos);
		return leftDist > rightDist;
		});
}

void GravityRender::RenderPoints()
{
	using namespace Engine;

	if (!typeDraw.point || _renderBodies.empty()) {
		return;
	}
	
	{
		pointBuffer.Bind(true, true, Color());

		std::vector<float> points;
		points.reserve(_renderBodies.size());

		for (const auto& body : _renderBodies) {
			points.emplace_back(body.pos.x);
			points.emplace_back(body.pos.y);
			points.emplace_back(body.pos.z);
		}

		float color[4] = { 1.f, 1.f, 1.f, 1.0f };
		const auto& shader = shaders::LineShaderSingle::Instance();
		shader.UseProgram();
		shader.SetColor(color);

		Draw::SetPointSize(5.f);
		Draw::RenderPoints(points.data(), points.size() / 3);
		pointBuffer.UnBind();
	}

	{
		//resultBuffer.Bind(true, true, Color());
		resultBuffer.Bind(true, true, Color(0.1f, 0.2f, 0.3f, 1.f));
		shaders::AccumShaderSingle::Instance().UseProgram(traceDecay, traceBuffer.GetTexture(), pointBuffer.GetTexture());
		Draw::RenderTriangleFun(DrawBuffer::QuadVAO(), 4);
		resultBuffer.UnBind();
		shaders::AccumShaderSingle::Instance().EndProgram();

		if (saveBufferToFile) {
			traceBuffer.Save("traceBuffer.png");
			pointBuffer.Save("pointBuffer.png");
			resultBuffer.Save("resultBuffer.png");
			saveBufferToFile = false;
		}

		std::swap(traceBuffer, resultBuffer);
	}
	
	{
		tempBuffer.Bind();
		tempBuffer.UnBind();
	}

	Draw::ClearColor(0.1f, 0.2f, 0.3f, 1.f);
	DrawBuffer::Draw(resultBuffer);
	Draw::ClearDepth();
}

void GravityRender::RenderCoordinateGrid()
{
	using namespace Engine;
	shaders::LineShaderSingle::Instance().UseProgram();
	shaders::LineShaderSingle::Instance().SetColor(Color(1.f, 1.f, 1.f, 0.125f));

	auto& spatialGrid = SpatialGrid::Instance();
	Draw::RenderLines(spatialGrid.Data(), spatialGrid.Count() / 3);
}

void GravityRender::ClearPointBuffer()
{
	traceBuffer.Clear();
}

void GravityRender::RenderSprite()
{
	using namespace Engine;

	if (!typeDraw.sprite || _renderBodies.empty()) {
		return;
	}

	// TODO:
	if (!typeDraw.point) {
		Draw::ClearColor(0.1f, 0.2f, 0.3f, 1.f);
	}

	auto& shader = shaders::BaseShaderSingle::Instance();
	shader.UseProgram();

	Draw::ClearDepth();
	Draw::DepthTest(true);
	Draw::ActiveTexture(0);
	Draw::BindTexture(Texture::GetRef("Star.png").Id());

	const glm::vec3 camPos = Camera::GetLink().Pos();
	float farDist = glm::distance(camPos, _renderBodies.front().pos);
	float nearDist = glm::distance(camPos, _renderBodies.back().pos);
	float spaceDist = farDist - nearDist;
	float color[4] = { 1.f, 1.f, 1.f, 1.f };

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

		shader.SetColor(color);

		Draw::Render(SHAPES["Sprite", true, true].mesh);
	}

	/*if (typeDraw.spriteShader) {
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
	}*/
}
