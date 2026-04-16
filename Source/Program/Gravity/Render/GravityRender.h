// ◦ Xyz ◦
#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <Object/Framebuffer.h>
#include <Object/Framebuffer2.h>
#include "../Spaces/BodyData.h"

class GravityRender final
{
public:
	struct TypeDraw
	{
		bool sprite = false;
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

private:
	inline static std::vector<Body> _renderBodies;
	//inline static Framebuffer prevPointBuffer;
	//inline static Framebuffer pointBuffer;
	//inline static Framebuffer frameBuffer;

	inline static Engine::FrameBuffer2 pointBuffer;
	inline static Engine::FrameBuffer2 bufferA;
	inline static Engine::FrameBuffer2 bufferB;

public:
	inline static float alpha = 0.f;
	inline static float deltaAlphaTime = 0.f;
};
