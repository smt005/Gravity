// ◦ Xyz ◦
#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include "../Spaces/Body.h"

class GravityRender final
{
public:
	struct TypeDraw
	{
		bool model = true;
		bool sprite = false;
		bool spriteShader = false;
		bool point = false;

	};

public:
	static void Init();
	static void Render();
	static void GetBodyPositions(std::vector<float>& data);
	static void GetBodyPositions(std::vector<glm::vec3>& vec3Data);

public:
	inline static TypeDraw typeDraw;

private:
	inline static std::vector<float> _data;
	inline static std::vector<glm::vec3> _vec3Data;
};
