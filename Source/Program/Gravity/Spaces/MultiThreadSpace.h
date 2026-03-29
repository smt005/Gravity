// ◦ Xyz ◦
#pragma once

#include "Space.h"
#include <mutex>
#include <MyMath.h>
#include "BodyData.h"

class MultiThreadSpace final : public Space
{
private:
	struct Body final {
		float mass = 1.f;
		mystd::Vec3 pos;
		mystd::Vec3 force;
		mystd::Vec3 velocity;
		float radius = 0.f;
		void* colapseData = nullptr;

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

	float GetSubProgress() const override;
	float GetProgress() const override;

	void Update() override;
	void UpdateInternal();

private:
	std::atomic<bool> _isBusy;
	std::atomic<int> _countObject;
	std::atomic<float> _subProcess;
	std::atomic<float> _process;
	std::vector<BodyData> _bufferBodies;
	std::vector<Body> _bodies;
	mutable std::mutex _mutex;
	mutable std::mutex _bufferMutex;
};
