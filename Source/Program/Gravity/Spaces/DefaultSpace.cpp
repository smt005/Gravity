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

	auto& debugContext = DebugContext::Instance();
	debugContext.Clean();
	debugContext.deltaTimes.clear();
	Engine::TimeRefHundler timer(DebugContext::Instance().deltaTimes.emplace_back());

	const float angleSpeed = SpaceManager::offsetIteration.load() * SpaceManager::countOfIteration.load() * angleSpeedFactor;
	const glm::vec3 rotationAxis(0.f, 0.f, 1.f);
	const glm::quat rotationQuat = glm::angleAxis(angleSpeed, rotationAxis);

	for (auto& body : _bodies) {
		const auto newPos = rotationQuat * glm::vec3(body.pos.X(), body.pos.Y(), body.pos.Z());
		body.pos = { newPos.x, newPos.y, newPos.z };
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
		return GravityRender::Body{ Diameter(body.mass), { body.pos.X(), body.pos.Y(), body.pos.Z()} };
	});
}

std::vector<BodyData> Default::GetBodies()
{
	std::vector<BodyData> bodies;
	bodies.reserve(_bodies.size());
	
	for (const Body& body : _bodies) {
		const auto& pos = body.pos;
		const auto& vel = body.velocity;
		bodies.emplace_back(body.mass, pos.X(), pos.Y(), pos.Z(), vel.X(), vel.Y(), vel.Z());
	}

	return bodies;
}
