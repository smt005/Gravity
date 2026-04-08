// ◦ Xyz ◦

#include "MainThreadSpace.h"
#include <atomic>
//#include <glm/gtc/quaternion.hpp>
#include <Callback/Callback.h>
#include "../DebugContext.h"
#include "SpaceManager.h"

using namespace Spaces;

MainThread::MainThread()
{
	params.emplace_back(false, "for...");
	LOG("Space: {} MainThread", typeid(this).hash_code());
}


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
		_velocities.emplace_back(body.velocity.x, body.velocity.y, body.velocity.z);
	}
}

void MainThread::Bodies(std::vector<GravityRender::Body>& bodies)
{
	bodies.clear();
	bodies.resize(_bodies.size());

	std::transform(_bodies.begin(), _bodies.end(), bodies.begin(), [](const Body& body) {
		return GravityRender::Body{ Diameter(body.mass), glm::vec3(body.pos.X(), body.pos.Y(), body.pos.Z()) };
	});
}

std::vector<BodyData> MainThread::GetBodies()
{
	std::vector<BodyData> bodies;
	const size_t size = _bodies.size();
	bodies.reserve(size);
	 
	for (size_t i = 0; i < size; ++i) {
		const auto& pos = _bodies[i].pos;
		const auto& vel = _velocities[i];
		bodies.emplace_back(_bodies[i].mass, pos.X(), pos.Y(), pos.Z(), vel.X(), vel.Y(), vel.Z());
	}

	return bodies;
}

void MainThread::Update()
{
	auto& debugContext = DebugContext::Instance();
	debugContext.Clean();
	debugContext.deltaTimes.clear();

	const size_t size = _bodies.size();
	std::vector<mystd::Vec3> forces(size);
	std::deque<Colapce> colapses;
	std::vector<Colapce*> colapseOfBodies(size, nullptr);

	if (!params[0].first) {
		UpdateForce(0, size, size, forces, colapses, colapseOfBodies);
	}
	else {
		UpdateForceParamTrue(0, size, size, forces, colapses, colapseOfBodies);
	}

	ColapceBodies(forces, colapses, colapseOfBodies);
	UpdatePositions(forces);
	
	debugContext.countObject = _bodies.size();
}

void MainThread::UpdateForce(size_t iBegin, size_t iEnd, size_t size, std::vector<mystd::Vec3>& forces, std::deque<Colapce>& colapses, std::vector<Colapce*>& colapseOfBodies)
{
	Engine::TimeRefHundler timer(DebugContext::Instance().deltaTimes.emplace_back(), "Update force");

	for (size_t i = iBegin; i < iEnd; ++i) {
		auto& bodyForce = forces[i];

		for (size_t j = i + 1; j < size; ++j) {	
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
				const auto forceDirection = direction.Normalized();
				const auto force = forceDirection * forceMagnitude;
				bodyForce += force;
				forces[j] -= force;
			}
		}
	}
}

void MainThread::UpdateForceParamTrue(size_t iBegin, size_t iEnd, size_t size, std::vector<mystd::Vec3>& forces, std::deque<Colapce>& colapses, std::vector<Colapce*>& colapseOfBodies)
{
	Engine::TimeRefHundler timer(DebugContext::Instance().deltaTimes.emplace_back(), "Update force");

	for (size_t i = iBegin; i < iEnd; ++i) {
		auto& bodyForce = forces[i];

		for (size_t j = 0; j < size; ++j) {
			if (i == j) {
				continue;
			}

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
				const auto forceDirection = direction.Normalized();
				bodyForce += forceDirection * forceMagnitude;
			}
		}
	}
}

void MainThread::ColapceBodies(std::vector<mystd::Vec3>& forces, std::deque<Colapce>& colapses, std::vector<Colapce*>& colapseOfBodies)
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

void MainThread::UpdatePositions(const std::vector<mystd::Vec3>& forces)
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
