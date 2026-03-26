// ◦ Xyz ◦

#include "DefaultSpace.h"
#include <deque>
#include <numbers>
#include <glm/vec3.hpp>
#include <glm/ext/vector_common.hpp>
#include <glm/gtc/quaternion.hpp>
#include <Common/Common.h>
#include "../DebugContext.h"
#include "SpaceManager.h"
#include <Logs.h>

void DefaultSpace::Clear()
{
	_bodies.clear();
}

void DefaultSpace::Update() {
	if (SpaceManager::countOfIteration == 0) {
		return;
	}

	static volatile float angleSpeedFactor = 0.001f;
	const float angleSpeed = static_cast<float>(SpaceManager::offsetIteration) * static_cast<float>(SpaceManager::countOfIteration) * angleSpeedFactor;
	const glm::vec3 rotationAxis(0.f, 0.f, 1.f);
	const glm::quat rotationQuat = glm::angleAxis(angleSpeed, rotationAxis);

	for (auto& body : _bodies) {
		body.pos = rotationQuat * body.pos;
	}
}

void DefaultSpace::AddBody(Body& body)
{
	_bodies.emplace_back(body);
}

void DefaultSpace::AddBody(Body&& body)
{
	_bodies.emplace_back(std::forward<Body>(body));
}

void DefaultSpace::AddBodies(std::vector<Body>& bodies)
{
	_bodies.append_range(bodies);
}

void DefaultSpace::AddBodies(std::vector<Body>&& bodies)
{
	if (_bodies.empty()) {
		std::swap(_bodies, bodies);
	}
	else {
		_bodies.append_range(bodies);
	}
}

void DefaultSpace::Bodies(std::vector<Body>& bodies) const
{
	bodies = _bodies;
}
