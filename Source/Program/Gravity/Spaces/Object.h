// ◦ Xyz ◦
#pragma once

#include <glm/vec3.hpp>

struct Object final {
	glm::vec3 pos;
	float mass = 1.f;
	glm::vec3 speed;
	glm::vec3 force;
	void* colapseData = nullptr;
	float minDist = std::numeric_limits<float>::max();
	float maxForce = std::numeric_limits<float>::min();

public:
	inline static float gForce = 1.f; // Гравитационная постоянная
};

