// ◦ Xyz ◦
#pragma once

#include <vector>
#include <glm/vec3.hpp>
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
	static void RenderSprite();
	static void ClearPointBuffer();

public:
	inline static TypeDraw typeDraw;
	inline static float scaleBody = 1.f;
	inline static float trace = 1.f;

private:
	inline static std::vector<Body> _renderBodies;

	inline static Engine::FrameBuffer pointBuffer;
	inline static Engine::FrameBuffer bufferA;
	inline static Engine::FrameBuffer bufferB;
	inline static float traceDecayTime = 0.f;
	inline static float traceDecay = 0.002f;
};
