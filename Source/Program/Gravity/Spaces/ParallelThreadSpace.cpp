// ◦ Xyz ◦

#include "ParallelThreadSpace.h"
#include <deque>
#include <thread>
#include <glm/gtc/quaternion.hpp>
#include "../DebugContext.h"
#include "SpaceManager.h"

void ParallelThreadSpace::Clear()
{
	_bufferBodies.clear();

	std::lock_guard lockMutex(_mutex);
	_bodies.clear();
}

void ParallelThreadSpace::AddBody(Body& body)
{
	std::lock_guard lockMutex(_mutex);
	_bodies.emplace_back(body);
}

void ParallelThreadSpace::AddBody(Body&& body)
{
	std::lock_guard lockMutex(_mutex);
	_bodies.emplace_back(std::forward<Body>(body));
}

void ParallelThreadSpace::AddBodies(std::vector<Body>& bodies)
{
	std::lock_guard lockMutex(_mutex);
	_bodies.append_range(bodies);
}

void ParallelThreadSpace::AddBodies(std::vector<Body>&& bodies)
{
	std::lock_guard lockMutex(_mutex);
	if (_bodies.empty()) {
		std::swap(_bodies, bodies);
	}
	else {
		_bodies.append_range(bodies);
	}
}

void ParallelThreadSpace::Bodies(std::vector<Body>& bodies) const
{
	/*std::unique_lock lockMutex(_mutex);
	if (lockMutex.try_lock()) {
		bodies = _bodies;
	}*/
	bodies = _bufferBodies;
}

void ParallelThreadSpace::GetBodyPositions(std::vector<float>& positions) const
{
	std::vector<float> tempPositions;

	{
		std::lock_guard lockMutex(_mutex);
		tempPositions.reserve(_bodies.size() * 3);

		for (const auto& body : _bodies) {
			tempPositions.emplace_back(body.pos.x);
			tempPositions.emplace_back(body.pos.y);
			tempPositions.emplace_back(body.pos.z);
		}
	}

	std::swap(positions, tempPositions);
}

void ParallelThreadSpace::GetBodyPositions(std::vector<glm::vec3>& positions) const
{
	std::vector<glm::vec3> tempPositions;

	{
		std::lock_guard lockMutex(_mutex);
		tempPositions.reserve(_bodies.size());

		for (const auto& body : _bodies) {
			tempPositions.emplace_back(body.pos);
		}
	}

	std::swap(positions, tempPositions);
}

void ParallelThreadSpace::Update()
{
	if (SpaceManager::countOfIteration == 0 || _isBusy.load()) {
		return;
	}

	auto& debugContext = DebugContext::Instance();
	debugContext.Clean();

	debugContext.subProgress = 0.f;
	debugContext.progress = 0.f;

	const float deltaTime = (float)SpaceManager::offsetIteration;

	std::thread th([this, deltaTime]() {
		_isBusy.store(true);

		for (int iter = 1; iter <= SpaceManager::countOfIteration; ++iter)
		{
			std::lock_guard lockMutex(_mutex);
			UpdateColapse();
			UpdateForce();
			UpdateSpeed(deltaTime);
			UpdatePos(deltaTime);
		}

		_bufferBodies = _bodies;
		_isBusy.store(false);
	});

	th.detach();

	//debugContext.progress = static_cast<float>(iter / SpaceManager::countOfIteration);
	//debugContext.deltaTime = deltaTime;

	/*{
		std::lock_guard lockMutex(_mutex);
		_bufferBodies = _bodies;
	}*/
	

	//debugContext.countObject = _bufferBodies.size();
}

void ParallelThreadSpace::UpdateColapse()
{
	if (!SpaceManager::collapseBodies) {
		return;
	}

	struct Colapce {
		int objectIndex = -1;
		glm::vec3 sumPos;
		glm::vec3 sumVelocity;
		float sumMass = 0;
	};

	std::deque<Colapce> colapses;
	const size_t count = _bodies.size();

	for (size_t i = 0; i < count; ++i) {
		_bodies[i].colapseData = nullptr;
	}

	for (size_t i = 0; i < count; ++i) {
		for (size_t j = 0; j < count; ++j) {
			if (i == j) {
				continue;
			}

			const float dist = glm::distance(_bodies[i].pos, _bodies[j].pos);

			if (dist <= (_bodies[i].Radius() + _bodies[j].Radius())) {
				Colapce* colapcePtr = static_cast<Colapce*>(_bodies[i].colapseData);
				if (!colapcePtr) {
					colapcePtr = static_cast<Colapce*>(_bodies[j].colapseData);
				}

				if (!colapcePtr) {
					colapcePtr = &colapses.emplace_back();
				}

				if (!_bodies[i].colapseData) {
					_bodies[i].colapseData = colapcePtr;
					colapcePtr->sumPos += _bodies[i].pos * _bodies[i].mass;
					colapcePtr->sumVelocity += _bodies[i].velocity * _bodies[i].mass;
					colapcePtr->sumMass += _bodies[i].mass;
					colapcePtr->objectIndex = i;
				}
				if (!_bodies[j].colapseData) {
					_bodies[j].colapseData = colapcePtr;
					colapcePtr->sumPos += _bodies[j].pos * _bodies[j].mass;
					colapcePtr->sumVelocity += _bodies[j].velocity * _bodies[j].mass;
					colapcePtr->sumMass += _bodies[j].mass;
				}
			}
		}
	}

	for (Colapce& colapses : colapses) {
		_bodies[colapses.objectIndex].pos = colapses.sumPos / colapses.sumMass;
		_bodies[colapses.objectIndex].velocity = colapses.sumVelocity / colapses.sumMass;
		_bodies[colapses.objectIndex].mass = colapses.sumMass;
		_bodies[colapses.objectIndex].colapseData = nullptr;
	}

	const auto removeIt = std::remove_if(_bodies.begin(), _bodies.end(), [](const auto& object) {
		return object.colapseData;
		});

	size_t removed = std::distance(removeIt, _bodies.end());
	_bodies.erase(removeIt, _bodies.end());

	colapses.clear();
}

void ParallelThreadSpace::UpdateForce()
{
	// TODO:
	auto& bodies = _bodies;
	static glm::vec3 noForce(0.0f);

	for (auto& body : bodies) {
		body.force = noForce;
	}

	const size_t count = bodies.size();

	for (size_t i = 0; i < count; ++i) {
		for (size_t j = i + 1; j < count; ++j) {
			const glm::vec3 direction = _bodies[j].pos - _bodies[i].pos;
			float distanceSquared = glm::length(direction);
			if (distanceSquared < 1.0f) {
				distanceSquared = 1.f;
				//throw; // TODO:
			}

			const float distance = std::sqrt(distanceSquared);
			const float forceMagnitude = Body::constantGravity * _bodies[i].mass * _bodies[j].mass / (distanceSquared * distanceSquared);
			const glm::vec3 forceDirection = glm::normalize(direction);
			const glm::vec3 force = forceMagnitude * forceDirection;

			_bodies[i].force += force;
			_bodies[j].force -= force;
		}
	}
}

void ParallelThreadSpace::UpdateSpeed(float deltaTime)
{
	for (auto& body : _bodies) {
		glm::vec3 acceleration = body.force / body.mass;
		body.velocity += acceleration * deltaTime;
	}
}

void ParallelThreadSpace::UpdatePos(float deltaTime)
{
	for (auto& body : _bodies) {
		body.pos += body.velocity * deltaTime;
	}
}
