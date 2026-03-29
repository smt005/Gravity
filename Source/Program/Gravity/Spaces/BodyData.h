// ◦ Xyz ◦
#pragma once

#include <numbers>
#include <glm/vec3.hpp>

struct BodyData
{
	float mass;
	glm::vec3 pos;
	glm::vec3 velocity;

	BodyData() = default;

	template <typename TVec3>
	BodyData(float _mass, const TVec3& _pos, const TVec3& _velocity)
		: mass(_mass)
		, pos(_pos.x, _pos.y, _pos.z)
		, velocity(_velocity.x, _velocity.y, _velocity.z)
	{}
	
	BodyData(float _mass, float posX, float posY, float posZ, float velX, float velY, float velZ)
		: mass(_mass)
		, pos(posX, posY, posZ)
		, velocity(velX, velY, velZ)
	{
	}

	float Radius() const {
		return std::cbrt((3.f * mass) / (4.f * std::numbers::pi));
	}

	float Diameter() const {
		return Radius() * 2.f;
	}
};
