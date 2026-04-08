// ◦ Xyz ◦
#pragma once

#include <vector>
#include <deque>
#include <mutex>
#include <atomic>
#include <MyMath.h>
#include "Space.h"

namespace Spaces
{
	class MultiThread : public Space
	{
	protected:
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
			{
			}
		};

		struct Colapce {
			Colapce(size_t _objectIndex) : objectIndex(_objectIndex) {}
			size_t objectIndex;
			float sumMass = 0.f;
			mystd::Vec3 sumPos;
			mystd::Vec3 sumVelocity;
		};

	public:
		MultiThread();
		void Clear() override;
		void Update() override;
		void AddBody(const BodyData& body) override;
		void AddBodies(const std::vector<BodyData>& bodies) override;
		void Bodies(std::vector<GravityRender::Body>& bodies) override;
		std::vector<BodyData> GetBodies() override;

	private:
		void UpdateForce(size_t iBegin, size_t iEnd, size_t size);
		void ColapceBodies();
		void UpdatePositions();
		void CopyBodies();
		void ApplyForce();

	protected:
		double _beginTime;
		std::vector<Body> _bodies;
		std::vector<GravityRender::Body> _renderBodies;
		std::deque<Colapce> _colapses;
		std::vector<Colapce*> _colapseOfBodies;

		mutable std::atomic<int> _countProcessing;
		mutable std::mutex _mutex;
		mutable std::mutex _copyMutex;
		mutable std::condition_variable _conditionMutex;
	};
}
