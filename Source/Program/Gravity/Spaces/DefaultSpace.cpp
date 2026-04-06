// ◦ Xyz ◦

#include "DefaultSpace.h"
#include <glm/gtc/quaternion.hpp>
#include "../DebugContext.h"
#include "SpaceManager.h"

using namespace Spaces;

void Default::Clear()
{
	_bodies.clear();
}

void Default::Update() {
	static volatile float angleSpeedFactor = 0.001f;
	const float angleSpeed = SpaceManager::offsetIteration.load() * SpaceManager::countOfIteration.load() * angleSpeedFactor;
	const glm::vec3 rotationAxis(0.f, 0.f, 1.f);
	const glm::quat rotationQuat = glm::angleAxis(angleSpeed, rotationAxis);

	for (auto& body : _bodies) {
		body.pos = rotationQuat * body.pos;
	}
}

void Default::AddBody(const BodyData& body)
{
	_bodies.emplace_back(body);
}

void Default::AddBodies(const std::vector<BodyData>& bodies)
{
	_bodies.append_range(bodies);
}

void Default::Bodies(std::vector<GravityRender::Body>& bodies)
{
	bodies.clear();
	bodies.resize(_bodies.size());

	std::transform(_bodies.begin(), _bodies.end(), bodies.begin(), [](const Body& body) {
		return GravityRender::Body{ Diameter(body.mass), body.pos };
	});
}

std::vector<BodyData> Default::GetBodies()
{
	std::vector<BodyData> bodies;
	bodies.reserve(_bodies.size());
	
	for (const Body& body : _bodies) {
		const auto& pos = body.pos;
		const auto& vel = body.velocity;
		bodies.emplace_back(body.mass, pos.x, pos.y, pos.z, vel.x, vel.y, vel.z);
	}

	return bodies;
}
