// ◦ Xyz ◦
#pragma once

#include "Space.h"
#include <mutex>
#include <deque>
#include <MyMath.h>
#include "BodyData.h"

class MultiThreadSpace final : public Space
{
private:
	struct Colapce {
		int objectIndex = -1;
		mystd::Vec3 sumPos;
		mystd::Vec3 sumVelocity;
		float sumMass = 0.f;
	};

	struct Body final {
		float mass = 1.f;
		mystd::Vec3 pos;
		mystd::Vec3 force;
		mystd::Vec3 velocity;
		float radius = 0.f;
		Colapce* colapseData = nullptr;

	public:
		Body() = default;
		Body(const BodyData& bodyData)
			: mass(bodyData.mass)
			, pos(bodyData.pos.x, bodyData.pos.y, bodyData.pos.z)
			, velocity(bodyData.velocity.x, bodyData.velocity.y, bodyData.velocity.z)
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
	MultiThreadSpace() = default;
	void Clear() override;
	void AddBody(const BodyData& body) override;
	void AddBodies(const std::vector<BodyData>& bodies) override;
	void Bodies(std::vector<BodyData>& bodies) override;
	std::vector<BodyData> GetBodies() override;

	ThreadType GetThreadType() override {
		return Space::ThreadType::PARALEL;
	}

	void Update() override;
	void UpdateInternal();
	void Iterations(size_t iBegin, size_t iEnd, size_t count, std::deque<Colapce>& colapses, float dProgress);
	void IterationsNoMutex(size_t iBegin, size_t iEnd, size_t count, std::deque<Colapce>& colapses, float dProgress);

private:
	std::atomic<bool> _isBusy;
	std::atomic<int> _countObject;
	std::atomic<float> _subProcess;
	std::atomic<float> _process;
	std::vector<BodyData> _bufferBodies;
	std::vector<Body> _bodies;
	mutable std::mutex _mutex;
	mutable std::mutex _bufferMutex;
	mutable std::mutex _iterationMutex;
};
