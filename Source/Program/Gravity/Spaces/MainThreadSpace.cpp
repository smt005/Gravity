// ◦ Xyz ◦

#include "MainThreadSpace.h"
#include <glm/gtc/quaternion.hpp>
#include "../DebugContext.h"
#include "SpaceManager.h"

using namespace Spaces;

void MainThread::Clear()
{
	_bodies.clear();
	_velocities.clear();
}

void MainThread::AddBody(const BodyData& body)
{
	_bodies.emplace_back(body);
}

void MainThread::AddBodies(const std::vector<BodyData>& bodies)
{
	const size_t size = bodies.size();

	for (const auto& body : bodies) {
		_bodies.emplace_back(body);
		_velocities.emplace_back(body.velocity);
	}
}

void MainThread::Bodies(std::vector<GravityRender::Body>& bodies)
{
	bodies.clear();
	bodies.resize(_bodies.size());

	std::transform(_bodies.begin(), _bodies.end(), bodies.begin(), [](const Body& body) {
		return GravityRender::Body{ Diameter(body.mass), body.pos };
	});
}

std::vector<BodyData> MainThread::GetBodies()
{
	std::vector<BodyData> bodies;
	const size_t size = _bodies.size();
	bodies.reserve(size);
	
	for (size_t i = 0; i < size; ++i) {
		bodies.emplace_back(_bodies[i].mass, _bodies[i].pos, _velocities[i]);
	}

	return bodies;
}

void MainThread::Update()
{
	const size_t size = _bodies.size();
	std::vector<glm::vec3> forces(size, glm::vec3(0.f, 0.f, 0.f));
	std::deque<Colapce> colapses;
	std::vector<Colapce*> colapseOfBodies(size, nullptr);

	UpdateForce(0, size, size, forces, colapses, colapseOfBodies);
	ColapceBodies(forces, colapses, colapseOfBodies);
	UpdatePositions(forces);
}

void MainThread::UpdateForce(size_t iBegin, size_t iEnd, size_t size, std::vector<glm::vec3>& forces, std::deque<Colapce>& colapses, std::vector<Colapce*>& colapseOfBodies)
{
	for (size_t i = iBegin; i < iEnd; ++i) {
		for (size_t j = 0; j < size; ++j) {
			if (i == j) {
				continue;
			}

			auto& force = forces[i];
			const glm::vec3 direction = _bodies[j].pos - _bodies[i].pos;
			const float distance = glm::length(direction);

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
					colapcePtr->sumVelocity += _velocities[i] * body.mass;
				}
				if (!colapseOfBodies[j]) {
					colapseOfBodies[j] = colapcePtr;
					const auto& body = _bodies[j];
					colapcePtr->sumPos += body.pos * body.mass;
					colapcePtr->sumMass += body.mass;
					colapcePtr->sumVelocity += _velocities[j] * body.mass;
				}
			}
			else {
				const float forceMagnitude = Space::constantGravity * _bodies[i].mass * _bodies[j].mass / std::pow(distance, 2);
				const auto forceDirection = glm::normalize(direction);
				force += forceDirection * forceMagnitude;
			}
		}
	}
}

void MainThread::ColapceBodies(std::vector<glm::vec3>& forces, std::deque<Colapce>& colapses, std::vector<Colapce*>& colapseOfBodies)
{
	for (Colapce& colapses : colapses) {
		auto& body = _bodies[colapses.objectIndex];
		body.mass = colapses.sumMass;
		body.pos = colapses.sumPos / colapses.sumMass;
		body.radius = Diameter(body.mass);
		_velocities[colapses.objectIndex] = colapses.sumVelocity / colapses.sumMass;
		colapseOfBodies[colapses.objectIndex] = nullptr;
	}

	size_t size = _bodies.size();
	size_t iLast = size - 1;
	size_t i = 0;

	while (i < iLast) {
		if (colapseOfBodies[i]) {
			forces[colapseOfBodies[i]->objectIndex] += forces[i];
			colapseOfBodies[i] = colapseOfBodies[iLast];
			_bodies[i] = _bodies[iLast];
			_velocities[i] = _velocities[iLast];
			forces[i] = forces[iLast];
			--iLast;
		}
		else {
			++i;
		}
	}

	size = iLast + 1;
	_bodies.resize(size);
	_velocities.resize(size);
	forces.resize(size);
}

void MainThread::UpdatePositions(const std::vector<glm::vec3>& forces)
{
	volatile static float velocityFactor = 1.f;
	const float deltaTime = SpaceManager::offsetIteration.load() * velocityFactor;
	const size_t size = _bodies.size();

	for (size_t i = 0; i < size; ++i) {
		const auto acceleration = forces[i] / _bodies[i].mass;
		_velocities[i] += acceleration * deltaTime;
		_bodies[i].pos += _velocities[i] * deltaTime;
	}
}
