// ◦ Xyz ◦
#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <Draw/Draw.h>
#include <Object/FrameBuffer.h>
#include "../Spaces/BodyData.h"

class GravityRender final
{
public:
	struct TypeDraw
	{
		bool sprite = true;
		bool point = true;
		bool model = false;
		bool spriteShader = false;
	};

	struct Body
	{
		float diameter;
		glm::vec3 pos;
	};

public:
	static void Init();
	static void Update(double deltaTime);
	static void Render();
	static void PrepareRender();
	static void RenderPoints();
	static void RenderCoordinateGrid();
	static void RenderSprite();
	static void ClearPointBuffer();
	static void SaveScreenshot();

public:
	inline static TypeDraw typeDraw;
	inline static float scaleBody = 2.f;
	inline static float trace = 1.f;

private:
	//inline static Engine::Color clearColor = Engine::Color(0.1f, 0.2f, 0.3f, 1.f);
	inline static Engine::Color clearColor = Engine::Color(0.f, 0.f, 0.f, 1.f);
	inline static std::vector<Body> _renderBodies;

	inline static Engine::FrameBuffer pointBuffer;
	inline static Engine::FrameBuffer resultBuffer;
	inline static Engine::FrameBuffer traceBuffer;
	inline static Engine::FrameBuffer mainBuffer;
	inline static float traceDecayTime = 0.f;
	inline static float traceDecay = 0.002f;
	inline static bool saveBuffersToFile = false;
};
