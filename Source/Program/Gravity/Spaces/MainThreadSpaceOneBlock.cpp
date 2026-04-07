// ◦ Xyz ◦

#include "MainThreadSpaceOneBlock.h"
#include <atomic>
//#include <glm/gtc/quaternion.hpp>
#include <Callback/Callback.h>
#include "../DebugContext.h"
#include "SpaceManager.h"

using namespace Spaces;

MainThreadSpaceOneBlock::MainThreadSpaceOneBlock()
{
	params.emplace_back(false, "for...");
}

void MainThreadSpaceOneBlock::Clear()
{
	_bodies.clear();
}

void MainThreadSpaceOneBlock::AddBody(const BodyData& body)
{
	_bodies.emplace_back(body);
}

void MainThreadSpaceOneBlock::AddBodies(const std::vector<BodyData>& bodies)
{
	const size_t size = bodies.size();

	for (const auto& body : bodies) {
		_bodies.emplace_back(body);
	}
}

void MainThreadSpaceOneBlock::Bodies(std::vector<GravityRender::Body>& bodies)
{
	bodies.clear();
	bodies.resize(_bodies.size());

	std::transform(_bodies.begin(), _bodies.end(), bodies.begin(), [](const Body& body) {
		return GravityRender::Body{ Diameter(body.mass), glm::vec3(body.pos.X(), body.pos.Y(), body.pos.Z()) };
	});
}

std::vector<BodyData> MainThreadSpaceOneBlock::GetBodies()
{
	std::vector<BodyData> bodies;
	const size_t size = _bodies.size();
	bodies.reserve(size);
	 
	for (size_t i = 0; i < size; ++i) {
		const auto& pos = _bodies[i].pos;
		const auto& vel = _bodies[i].velocity;
		bodies.emplace_back(_bodies[i].mass, pos.X(), pos.Y(), pos.Z(), vel.X(), vel.Y(), vel.Z());
	}

	return bodies;
}

void MainThreadSpaceOneBlock::Update()
{
	auto& debugContext = DebugContext::Instance();
	Engine::TimeHundler timer("Tag00");

	for (auto& body : _bodies) {
		body.force = { 0.f, 0.f, 0.f };
	}
	const size_t size = _bodies.size();
	std::deque<Colapce> colapses;
	std::vector<Colapce*> colapseOfBodies(size, nullptr);

	debugContext.deltaTimes[0] = timer.GetDeltaTime();

	if (!params[0].first) {
		UpdateForce(0, size, size, colapses, colapseOfBodies);
	}
	else {
		UpdateForceParamTrue(0, size, size, colapses, colapseOfBodies);
	}

	ColapceBodies(colapses, colapseOfBodies);
	UpdatePositions();
	
	debugContext.countObject = _bodies.size();
}

void MainThreadSpaceOneBlock::UpdateForce(size_t iBegin, size_t iEnd, size_t size, std::deque<Colapce>& colapses, std::vector<Colapce*>& colapseOfBodies)
{
	Engine::TimeRefHundler timer(DebugContext::Instance().deltaTimes[1], "Tag01");

	for (size_t i = iBegin; i < iEnd; ++i) {
		for (size_t j = i + 1; j < size; ++j) {
			auto& force = _bodies[i].force;
			const auto direction = _bodies[j].pos - _bodies[i].pos;
			const float distance = direction.Length();

			if (distance <= (_bodies[i].radius + _bodies[j].radius)) {
				Colapce* colapcePtr = colapseOfBodies[i];
				if (!colapcePtr) {
					colapcePtr = colapseOfBodies[j];
				}
				if (!colapcePtr) {
					colapcePtr = &colapses.emplace_back(i);
				}
				if (!colapseOfBodies[i]) {
					colapseOfBodies[i] = colapcePtr;
					const auto& body = _bodies[i];
					colapcePtr->sumPos += body.pos * body.mass;
					colapcePtr->sumMass += body.mass;
					colapcePtr->sumVelocity += body.velocity * body.mass;
				}
				if (!colapseOfBodies[j]) {
					colapseOfBodies[j] = colapcePtr;
					const auto& body = _bodies[j];
					colapcePtr->sumPos += body.pos * body.mass;
					colapcePtr->sumMass += body.mass;
					colapcePtr->sumVelocity += body.velocity * body.mass;
				}
			}
			else {
				const float forceMagnitude = Space::constantGravity * _bodies[i].mass * _bodies[j].mass / std::pow(distance, 2);
				const auto forceDirection = direction.Normalized();
				force += forceDirection * forceMagnitude;
				_bodies[j].force -= force;
			}
		}
	}
}

void MainThreadSpaceOneBlock::UpdateForceParamTrue(size_t iBegin, size_t iEnd, size_t size, std::deque<Colapce>& colapses, std::vector<Colapce*>& colapseOfBodies)
{
	Engine::TimeRefHundler timer(DebugContext::Instance().deltaTimes[1], "Tag01");

	for (size_t i = iBegin; i < iEnd; ++i) {
		for (size_t j = 0; j < size; ++j) {
			if (i == j) {
				continue;
			}

			auto& force = _bodies[i].force;
			const auto direction = _bodies[j].pos - _bodies[i].pos;
			const float distance = direction.Length();

			if (distance <= (_bodies[i].radius + _bodies[j].radius)) {
				Colapce* colapcePtr = colapseOfBodies[i];
				if (!colapcePtr) {
					colapcePtr = colapseOfBodies[j];
				}
				if (!colapcePtr) {
					colapcePtr = &colapses.emplace_back(i);
				}
				if (!colapseOfBodies[i]) {
					colapseOfBodies[i] = colapcePtr;
					const auto& body = _bodies[i];
					colapcePtr->sumPos += body.pos * body.mass;
					colapcePtr->sumMass += body.mass;
					colapcePtr->sumVelocity += body.velocity * body.mass;
				}
				if (!colapseOfBodies[j]) {
					colapseOfBodies[j] = colapcePtr;
					const auto& body = _bodies[j];
					colapcePtr->sumPos += body.pos * body.mass;
					colapcePtr->sumMass += body.mass;
					colapcePtr->sumVelocity += body.velocity * body.mass;
				}
			}
			else {
				const float forceMagnitude = Space::constantGravity * _bodies[i].mass * _bodies[j].mass / std::pow(distance, 2);
				const auto forceDirection = direction.Normalized();
				force += forceDirection * forceMagnitude;
			}
		}
	}
}

void MainThreadSpaceOneBlock::ColapceBodies(std::deque<Colapce>& colapses, std::vector<Colapce*>& colapseOfBodies)
{
	Engine::TimeRefHundler timer(DebugContext::Instance().deltaTimes[2], "Tag02");

	for (Colapce& colapses : colapses) {
		auto& body = _bodies[colapses.objectIndex];
		body.mass = colapses.sumMass;
		body.pos = colapses.sumPos / colapses.sumMass;
		body.radius = Diameter(body.mass);
		body.velocity = colapses.sumVelocity / colapses.sumMass;
		colapseOfBodies[colapses.objectIndex] = nullptr;
	}

	size_t size = _bodies.size();
	size_t iLast = size - 1;
	size_t i = 0;

	while (i < iLast) {
		if (colapseOfBodies[i]) {
			_bodies[colapseOfBodies[i]->objectIndex].force += _bodies[i].force;
			colapseOfBodies[i] = colapseOfBodies[iLast];
			_bodies[i] = _bodies[iLast];
			--iLast;
		}
		else {
			++i;
		}
	}

	size = iLast + 1;
	_bodies.resize(size);
}

void MainThreadSpaceOneBlock::UpdatePositions()
{
	Engine::TimeRefHundler timer(DebugContext::Instance().deltaTimes[3], "Tag03");

	volatile static float velocityFactor = 1.f;
	const float deltaTime = SpaceManager::offsetIteration.load() * velocityFactor;
	const size_t size = _bodies.size();

	for (size_t i = 0; i < size; ++i) {
		const auto acceleration = _bodies[i].force / _bodies[i].mass;
		_bodies[i].force += acceleration * deltaTime;
		_bodies[i].pos += _bodies[i].velocity * deltaTime;
	}
}
