// ◦ Xyz ◦
#pragma once

#include <numbers>
#include <glm/vec3.hpp>

// TODO
struct Object final {
	glm::vec3 pos;
	float mass = 1.f;
	glm::vec3 speed;
	glm::vec3 force;
	void* colapseData = nullptr;
	float minDist = std::numeric_limits<float>::max();
	float maxForce = std::numeric_limits<float>::min();

	float Scale() const {
		constexpr float val3div4 = (3.f / 4.f) / std::numbers::pi;
		float scale = std::cbrtf(val3div4 * mass);
		return scale;
	}

public:
	inline static float gForce = 1.f; // Гравитационная постоянная
};

