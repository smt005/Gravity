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

	float GetSubProgress() const override;
	float GetProgress() const override;

	void Update() override;
	void UpdateColapse();
	void UpdateForce();
	void UpdateSpeed(float deltaTime);
	void UpdatePos(float deltaTime);


protected:
	std::atomic<bool> _isBusy;
	std::atomic<float> _subProcess;
	std::atomic<float> _process;
	std::vector<Body> _bufferBodies;
	std::vector<Body> _bodies;
	mutable std::mutex _mutex;
};
