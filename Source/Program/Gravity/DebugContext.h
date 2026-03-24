// ◦ Xyz ◦
#pragma once

#include <Singletone.h>

//namespace gravity
//{
	class DebugContext : public mystd::Singletone<DebugContext>
	{
	public:
		DebugContext() = default;

		int countObject = 0;
		float diameterSpace = 0.f;
		float minForce = 0.f;
		float middleForce = 0.f;
		float maxForce = 0.f;
		float minSpeed = 0.f;
		float middleSpeed = 0.f;
		float maxSpeed = 0.f;
		float minMass = 0.f;
		float middleMass = 0.f;
		float maxMass = 0.f;
		float sumMass = 0.f;

		float constForce = 1.f;
		float constSpeed = 1.f;

		float deltaTime = 0.f;
		float subProgress = 0.f;
		float progress = 0.f;

		void Clean() {
			countObject = 0;
			diameterSpace = 0.f;
			minForce = std::numeric_limits<float>::max();
			middleForce = 0.f;
			maxForce = std::numeric_limits<float>::min();
			minSpeed = std::numeric_limits<float>::max();
			middleSpeed = 0.f;
			maxSpeed = std::numeric_limits<float>::min();
			minMass = std::numeric_limits<float>::max();
			middleMass = 0.f;
			maxMass = std::numeric_limits<float>::min();
			sumMass = 0.f;
		}
	};
//}
