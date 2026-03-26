// ◦ Xyz ◦
#pragma once

#include "Space.h"
#include <mutex>

class ParallelThreadSpace : public Space
{
public:
	ParallelThreadSpace() = default;
	void Clear() override;
	void AddBody(Body& body) override;
	void AddBody(Body&& body) override;
	void AddBodies(std::vector<Body>& bodies) override;
	void AddBodies(std::vector<Body>&& bodies) override;
	void Bodies(std::vector<Body>& bodies) const override;
	void GetBodyPositions(std::vector<float>& positions) const override;
	void GetBodyPositions(std::vector<glm::vec3>& positions) const override;

	void Update() override;
	void UpdateColapse();
	void UpdateForce();
	void UpdateSpeed(float deltaTime);
	void UpdatePos(float deltaTime);

protected:
	std::atomic<bool> _isBusy;
	std::vector<Body> _bufferBodies;
	std::vector<Body> _bodies;
	mutable std::mutex _mutex;
};
