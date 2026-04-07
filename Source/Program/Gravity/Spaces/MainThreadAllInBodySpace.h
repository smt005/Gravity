// ◦ Xyz ◦
#pragma once

#include <vector>
#include <deque>
#include <MyMath.h>
#include "DefaultSpace.h"

namespace Spaces
{
	class MainThreadAllInBody : public Default
	{
	private:
		struct Colapce {
			Colapce(size_t _objectIndex) : objectIndex(_objectIndex) {}
			size_t objectIndex;
			float sumMass = 0.f;
			mystd::Vec3 sumPos;
			mystd::Vec3 sumVelocity;
		};

	public:
		MainThreadAllInBody();
		void Update() override;

		void UpdateForce(size_t iBegin, size_t iEnd, size_t size, std::deque<Colapce>& colapses, std::vector<Colapce*>& colapseOfBodies);
		void UpdateForceParamTrue(size_t iBegin, size_t iEnd, size_t size, std::deque<Colapce>& colapses, std::vector<Colapce*>& colapseOfBodies);
		void ColapceBodies(std::deque<Colapce>& colapses, std::vector<Colapce*>& colapseOfBodies);
		void UpdatePositions();
	};
}
