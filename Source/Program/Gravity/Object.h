// ◦ Xyz ◦
#pragma once

#include <glm/vec3.hpp>

struct Object final {
	glm::vec3 pos;
	float weight = 1.f;
	glm::vec3 speed;
	glm::vec3 force;
	void* colapseData = nullptr;
	float minDist = std::numeric_limits<float>::max();
	float maxForce = std::numeric_limits<float>::min();

public:
	inline static float gForce = 0.1f; // Гравитационная постоянная
};

