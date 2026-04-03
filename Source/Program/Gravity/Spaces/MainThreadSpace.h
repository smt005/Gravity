// ◦ Xyz ◦
#pragma once

#include <vector>
#include <deque>
#include <string>
#include <glm/vec3.hpp>
#include <mystd_memory.h>
#include "BodyData.h"
#include "Space.h"

namespace Spaces
{
	class MainThread : public Space
	{
	private:
		struct Body final {
			float mass = 1.f;
			glm::vec3 pos;
			float radius = 0.f;

		public:
			Body() = default;
			Body(const BodyData& bodyData)
				: mass(bodyData.mass)
				, pos(bodyData.pos)
				, radius(Space::Radius(mass))
			{}
			float Radius() {
				return Diameter(mass);
			}
		};

		struct Colapce {
			Colapce(size_t _objectIndex) : objectIndex(_objectIndex) {}
			size_t objectIndex;
			float sumMass = 0.f;
			glm::vec3 sumPos;
			glm::vec3 sumVelocity;
		};

	public:
		void Clear() override;
		void Update() override;
		void AddBody(const BodyData& body) override;
		void AddBodies(const std::vector<BodyData>& bodies) override;
		void Bodies(std::vector<GravityRender::Body>& bodies) override;
		std::vector<BodyData> GetBodies() override;

	protected:
		std::vector<Body> _bodies;
		std::vector<glm::vec3> _velocities;
	};
}
