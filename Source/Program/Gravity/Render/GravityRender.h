// ◦ Xyz ◦
#pragma once

#include <vector>
#include <glm/vec3.hpp>
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
