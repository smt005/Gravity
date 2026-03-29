// ◦ Xyz ◦
#pragma once

#include <vector>
#include <string>
#include <glm/vec3.hpp>
#include <mystd_memory.h>
#include "BodyData.h"
#include "Space.h"

class DefaultSpace : public Space
{
private:
	struct Body final {
		float mass = 1.f;
		glm::vec3 pos = glm::vec3(0.f, 0.f, 0.f);
		glm::vec3 force = glm::vec3(0.f, 0.f, 0.f);
		glm::vec3 velocity = glm::vec3(0.f, 0.f, 0.f);
		void* colapseData = nullptr;

	public:
		Body() = default;
		Body(const BodyData& bodyData)
			: mass(bodyData.mass)
			, pos(bodyData.pos)
			, velocity(bodyData.velocity)
		{
		}

		float Radius() const {
			return std::cbrt((3.f * mass) / (4.f * std::numbers::pi));
		}

		float Diameter() const {
			return Radius() * 2.f;
		}
	};

public:
	DefaultSpace() = default;
	void Clear() override;
	void Update() override;
	void AddBody(const BodyData& body) override;
	void AddBodies(const std::vector<BodyData>& bodies) override;
	void Bodies(std::vector<BodyData>& bodies) override;

protected:
	std::vector<Body> _bodies;
};
