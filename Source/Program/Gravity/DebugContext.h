// ◦ Xyz ◦
#pragma once

#include <atomic>
#include <array>
#include <Singletone.h>

class DebugContext : public mystd::Singletone<DebugContext>
{
public:
	//DebugContext() = default;
	DebugContext() {
		LOG("DebugContext");
	}

	int countObject = 0;
	float diameterSpace = 0.f;
	float minForce = 0.f;
	float maxForce = 0.f;
	float minVelocity = 0.f;
	float maxVelocity = 0.f;
	float minMass = 0.f;
	float maxMass = 0.f;
	float sumMass = 0.f;

	float constForce = 1.f;
	float constSpeed = 1.f;

	std::vector<double> deltaTimes;
	float deltaTime = 0.f;
	float subProgress = 0.f;
	float progress = 0.f;

	void Clean() {
		countObject = 0.f;
		diameterSpace = 0.f;
		minForce = std::numeric_limits<float>::max();
		maxForce = std::numeric_limits<float>::min();
		minVelocity = std::numeric_limits<float>::max();
		maxVelocity = std::numeric_limits<float>::min();
		minMass = std::numeric_limits<float>::max();
		maxMass = std::numeric_limits<float>::min();
		sumMass = 0.f;

		subProgress = 0.f;
		progress = 0.f;
	}
};
