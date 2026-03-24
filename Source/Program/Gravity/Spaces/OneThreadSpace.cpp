// ◦ Xyz ◦

#include "OneThreadSpace.h"
#include <deque>
#include <Common/Common.h>
#include <Files/Settings.h>
#include "../DebugContext.h"
#include <Common/JsonHelper.h>
#include "SpaceManager.h"
#include "../../Temp/LogSpecification.h"
#include <Log.h>

void OneThreadSpace::UpdateColapse()
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
	const size_t count = _objects.size();

	for (size_t i = 0; i < count; ++i) {
		_objects[i].colapseData = nullptr;
	}

	for (size_t i = 0; i < count; ++i) {
		for (size_t j = 0; j < count; ++j) {
			if (i == j) {
				continue;
			}

			const float dist = glm::distance(_objects[i].pos, _objects[j].pos);

			if (dist <= (_objects[i].Radius() + _objects[j].Radius())) {
				Colapce* colapcePtr = static_cast<Colapce*>(_objects[i].colapseData);
				if (!colapcePtr) {
					colapcePtr = static_cast<Colapce*>(_objects[j].colapseData);
				}

				if (!colapcePtr) {
					colapcePtr = &colapses.emplace_back();
				}

				if (!_objects[i].colapseData) {
					_objects[i].colapseData = colapcePtr;
					colapcePtr->sumPos += _objects[i].pos * _objects[i].mass;
					colapcePtr->sumVelocity += _objects[i].velocity * _objects[i].mass;
					colapcePtr->sumMass += _objects[i].mass;
					colapcePtr->objectIndex = i;
				}
				if (!_objects[j].colapseData) {
					_objects[j].colapseData = colapcePtr;
					colapcePtr->sumPos += _objects[j].pos * _objects[j].mass;
					colapcePtr->sumVelocity += _objects[j].velocity * _objects[j].mass;
					colapcePtr->sumMass += _objects[j].mass;
				}
			}

			_objects[i].minDist = std::min(_objects[i].minDist, dist);
			_objects[j].minDist = std::min(_objects[j].minDist, dist);
		}

		subProgress = static_cast<float>((i + 1) / count);
	}

	for (Colapce& colapses : colapses) {
		_objects[colapses.objectIndex].pos = colapses.sumPos / colapses.sumMass;
		_objects[colapses.objectIndex].velocity = colapses.sumVelocity / colapses.sumMass;
		_objects[colapses.objectIndex].mass = colapses.sumMass;
		_objects[colapses.objectIndex].colapseData = nullptr;
	}

	const auto removeIt = std::remove_if(_objects.begin(), _objects.end(), [](const auto& object) {
		float maxDist = 1000;
		return object.colapseData || object.minDist > maxDist;
		});

	size_t removed = std::distance(removeIt, _objects.end());
	_objects.erase(removeIt, _objects.end());

	colapses.clear();
}

void OneThreadSpace::UpdateForce()
{
	// TODO:
	auto& bodies = _objects;
	static glm::vec3 noForce(0.0f);

	for (auto& body : bodies) {
		body.force = noForce;
	}

	const size_t count = bodies.size();

	for (size_t i = 0; i < count; ++i) {
		for (size_t j = i + 1; j < count; ++j) {
			const glm::vec3 direction = _objects[j].pos - _objects[i].pos;
			float distanceSquared = glm::length(direction);
			if (distanceSquared < 1.0f) {
				distanceSquared = 1.f;
				//throw; // TODO:
			}

			const float distance = std::sqrt(distanceSquared);
			const float forceMagnitude = Object::gForce * _objects[i].mass * _objects[j].mass / (distanceSquared * distanceSquared);
			const glm::vec3 forceDirection = glm::normalize(direction);
			const glm::vec3 force = forceMagnitude * forceDirection;

			_objects[i].force += force;
			_objects[j].force -= force;
		}

		progress = static_cast<float>((i + 1) / count);
	}
}

void OneThreadSpace::UpdateSpeed(float deltaTime)
{
	for (auto& obj : _objects) {
		glm::vec3 acceleration = obj.force / obj.mass;
		obj.velocity += acceleration * deltaTime;
	}
}

void OneThreadSpace::UpdatePos(float deltaTime)
{
	for (auto& obj : _objects) {
		obj.pos += obj.velocity * deltaTime;
	}
}

void OneThreadSpace::Update()
{
	if (SpaceManager::countOfIteration == 0) {
		return;
	}

	auto& debugContext = DebugContext::Instance();
	debugContext.Clean();

	debugContext.subProgress = 0.f;
	debugContext.progress = 0.f;

	for (int iter = 1; iter <= SpaceManager::countOfIteration; ++iter)
	{
		float deltaTime = (float)SpaceManager::offsetIteration;
		
		UpdateColapse();
		UpdateForce();
		UpdateSpeed(deltaTime);
		UpdatePos(deltaTime);

		debugContext.progress = static_cast<float>(iter / SpaceManager::countOfIteration);
		debugContext.deltaTime = deltaTime;
		debugContext.countObject = _objects.size();
	}
}

void OneThreadSpace::CollectDebugData() const
{
	Space::CollectDebugData();
}
