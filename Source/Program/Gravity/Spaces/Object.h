// ◦ Xyz ◦
#pragma once

#include <numbers>
#include <glm/vec3.hpp>

// TODO
struct Object final {
	glm::vec3 pos = glm::vec3(0.f, 0.f, 0.f);
	float mass = 1.f;
	glm::vec3 velocity = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 force = glm::vec3(0.f, 0.f, 0.f);
	void* colapseData = nullptr;
	float minDist = std::numeric_limits<float>::max();
	float maxForce = std::numeric_limits<float>::min();

	float Radius() const {
		return std::cbrt((3.f * mass) / (4.f * std::numbers::pi));
	}

	float Diameter () const {
		return Radius() * 2.f;
	}

public:
	inline static float gForce = 6.67430e-11f; //  1.f; // Гравитационная постоянная
};

