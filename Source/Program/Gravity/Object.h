// ◦ Xyz ◦
#pragma once

#include <glm/vec3.hpp>

struct Object final {
	glm::vec3 pos;
	float weight = 1.f;
	glm::vec3 force;

	inline static float gForce = 1.f; // Гравитационная постоянная
};

