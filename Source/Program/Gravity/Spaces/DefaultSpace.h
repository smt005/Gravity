// ◦ Xyz ◦
#pragma once

#include <vector>
#include <string>
#include <glm/vec3.hpp>
#include <mystd_memory.h>
#include "Body.h"
#include "Space.h"

class DefaultSpace : public Space
{
public:
	DefaultSpace() = default;
	void Clear() override;
	void Update() override;
	void AddBody(Body& body) override;
	void AddBody(Body&& body) override;
	void AddBodies(std::vector<Body>& bodies) override;
	void AddBodies(std::vector<Body>&& bodies) override;
	void Bodies(std::vector<Body>& bodies) const override;
	void GetBodyPositions(std::vector<float>& positions) const override;
	void GetBodyPositions(std::vector<glm::vec3>& positions) const override;

protected:
	std::vector<Body> _bodies;
};
