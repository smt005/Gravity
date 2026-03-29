// ◦ Xyz ◦

#include "MainThreadProtSpace.h"
#include <deque>
#include "../DebugContext.h"
#include "SpaceManager.h"

void MainThreadProtSpace::Update()
{
	if (SpaceManager::countOfIteration == 0) {
		return;
	}

	auto& debugContext = DebugContext::Instance();
	debugContext.Clean();

	debugContext.subProgress = 0.f;
	debugContext.progress = 0.f;
	float deltaTime = SpaceManager::offsetIteration.load();

	for (int iter = 1; iter <= SpaceManager::countOfIteration; ++iter)
	{	
		UpdateColapse();
		UpdateForce();
		UpdateSpeed(deltaTime);
		UpdatePos(deltaTime);

		debugContext.progress = static_cast<float>(iter / SpaceManager::countOfIteration);
		debugContext.deltaTime = deltaTime;
		debugContext.countObject = _bodies.size();
	}
}


void MainThreadProtSpace::UpdateColapse()
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

void MainThreadProtSpace::UpdateForce()
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

			const float forceMagnitude = Space::constantGravity * _bodies[i].mass * _bodies[j].mass / (distanceSquared * distanceSquared);
			const glm::vec3 forceDirection = glm::normalize(direction);
			const glm::vec3 force = forceMagnitude * forceDirection;

			_bodies[i].force += force;
			_bodies[j].force -= force;
		}
	}
}

void MainThreadProtSpace::UpdateSpeed(float deltaTime)
{
	for (auto& body : _bodies) {
		glm::vec3 acceleration = body.force / body.mass;
		body.velocity += acceleration * deltaTime;
	}
}

void MainThreadProtSpace::UpdatePos(float deltaTime)
{
	for (auto& body : _bodies) {
		body.pos += body.velocity * deltaTime;
	}
}
