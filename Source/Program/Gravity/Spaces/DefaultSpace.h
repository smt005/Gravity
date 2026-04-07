// ◦ Xyz ◦
#pragma once

#include <vector>
#include <string>
#include <glm/vec3.hpp>
#include <mystd_memory.h>
#include "BodyData.h"
#include "Space.h"

namespace Spaces
{
	class Default : public Space
	{
	struct Body final {
		float mass = 1.f;
		mystd::Vec3 pos;
		mystd::Vec3 velocity;
		mystd::Vec3 force = { 0.f, 0.f, 0.f };
		float radius = 0.f;

	public:
		Body() = default;
		Body(const BodyData& bodyData)
			: mass(bodyData.mass)
			, pos(bodyData.pos.x, bodyData.pos.y, bodyData.pos.z)
			, velocity(bodyData.velocity.x, bodyData.velocity.y, bodyData.velocity.z)
			, radius(Space::Radius(mass))
		{}
	};

	public:
		Default() {
			LOG("Space: {} Default", typeid(this).hash_code());
		}
		void Clear() override;
		void Update() override;
		void AddBody(const BodyData& body) override;
		void AddBodies(const std::vector<BodyData>& bodies) override;
		void Bodies(std::vector<GravityRender::Body>& bodies) override;
		std::vector<BodyData> GetBodies() override;

	protected:
		std::vector<Body> _bodies;
	};
}
