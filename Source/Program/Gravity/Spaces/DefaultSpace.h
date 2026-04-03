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
	private:
		struct Body final {
			float mass = 1.f;
			glm::vec3 pos;
			glm::vec3 velocity;
			glm::vec3 force;
			void* colapseData = nullptr;

		public:
			Body() = default;
			Body(const BodyData& bodyData)
				: mass(bodyData.mass)
				, pos(bodyData.pos)
				, velocity(bodyData.velocity)
			{}
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
	};
}
