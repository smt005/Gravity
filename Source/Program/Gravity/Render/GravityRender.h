// ◦ Xyz ◦
#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <Object/Framebuffer.h>
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
	static void Render();
	static void RenderToBuffer();
	static void RenderToScreen();

public:
	inline static TypeDraw typeDraw;
	inline static float scaleBody = 1.f;

private:
	inline static std::vector<Body> _renderBodies;
	inline static Framebuffer framebuffer;
};
