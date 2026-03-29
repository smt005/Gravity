// ◦ Xyz ◦
#pragma once

#include "Space.h"
#include <mutex>
#include "BodyData.h"

class ParallelThreadSpace : public Space
{
private:
	struct Body final {
		float mass = 1.f;
		glm::vec3 pos = glm::vec3(0.f, 0.f, 0.f);
		glm::vec3 force = glm::vec3(0.f, 0.f, 0.f);
		glm::vec3 velocity = glm::vec3(0.f, 0.f, 0.f);

		// TODO: Убрать/перенести в другое место
		void* colapseData = nullptr;
		float minDist = std::numeric_limits<float>::max();
		float maxForce = std::numeric_limits<float>::min();

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
	ParallelThreadSpace() = default;
	void Clear() override;
	void AddBody(const BodyData& body) override;
	void AddBodies(const std::vector<BodyData>& bodies) override;
	void Bodies(std::vector<BodyData>& bodies) override;

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
	std::vector<BodyData> _bufferBodies;
	std::vector<Body> _bodies;
	mutable std::mutex _mutex;
	mutable std::mutex _copyBifferMutex;
};
