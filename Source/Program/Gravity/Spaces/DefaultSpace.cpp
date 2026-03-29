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
	const float angleSpeed = SpaceManager::offsetIteration.load() * SpaceManager::countOfIteration.load() * angleSpeedFactor;
	const glm::vec3 rotationAxis(0.f, 0.f, 1.f);
	const glm::quat rotationQuat = glm::angleAxis(angleSpeed, rotationAxis);

	for (auto& body : _bodies) {
		body.pos = rotationQuat * body.pos;
	}
}

void DefaultSpace::AddBody(const BodyData& body)
{
	_bodies.emplace_back(body);
}

void DefaultSpace::AddBodies(const std::vector<BodyData>& bodies)
{
	_bodies.append_range(bodies);
}

void DefaultSpace::Bodies(std::vector<BodyData>& bodies)
{
	bodies.clear();
	bodies.reserve(_bodies.size());

	for (const auto& body : _bodies) {
		bodies.emplace_back(body.mass, body.pos.x, body.pos.y, body.pos.z, body.velocity.x, body.velocity.y, body.velocity.z);
	}
}

std::vector<BodyData> DefaultSpace::GetBodies()
{
	std::vector<BodyData> bodies;
	bodies.reserve(_bodies.size());

	for (const auto& body : _bodies) {
		bodies.emplace_back(body.mass, body.pos.x, body.pos.y, body.pos.z, body.velocity.x, body.velocity.y, body.velocity.z);
	}

	return bodies;
}
