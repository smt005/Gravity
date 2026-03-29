// ◦ Xyz ◦
#pragma once

#include "DefaultSpace.h"
#include <MyMath.h>

class MainThreadSpace : public Space
{
private:
	struct Body final {
		float mass = 1.f;
		mystd::Vec3 pos;
		mystd::Vec3 force;
		mystd::Vec3 velocity;
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
	void Clear() override;
	void AddBody(const BodyData& body) override;
	void AddBodies(const std::vector<BodyData>& bodies) override;
	void Bodies(std::vector<BodyData>& bodies) override;

	void Update() override;
	void UpdateColapse();
	void UpdateForce();
	void UpdateSpeed(float deltaTime);
	void UpdatePos(float deltaTime);

protected:
	std::vector<Body> _bodies;
};
