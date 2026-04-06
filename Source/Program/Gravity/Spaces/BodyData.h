// ◦ Xyz ◦
#pragma once

#include <numbers>
#include <glm/vec3.hpp>
#include <CudaWrapper.h>

struct BodyData
{
	float mass;
	glm::vec3 pos;
	glm::vec3 velocity;

	BodyData() = default;

	BodyData(const Cuda::Body& data)
		: mass(data.mass)
		, pos(data.pos.x, data.pos.y, data.pos.z)
		, velocity(data.velocity.x, data.velocity.y, data.velocity.z)
	{}

	BodyData(float _mass, float posX, float posY, float posZ, float velX, float velY, float velZ)
		: mass(_mass)
		, pos(posX, posY, posZ)
		, velocity(velX, velY, velZ)
	{}

	float Radius() const {
		return std::cbrt((3.f * mass) / (4.f * std::numbers::pi));
	}

	float Diameter() const {
		return Radius() * 2.f;
	}

	static std::vector<BodyData> Copy(const std::vector<Cuda::Body>& bodies);
};

std::ostream& operator<<(std::ostream& os, const BodyData& body);
