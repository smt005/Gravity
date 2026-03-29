// ◦ Xyz ◦

#include "MultiThreadSpace.h"
#include <deque>
#include <thread>
#include <glm/gtc/quaternion.hpp>
#include <Callback/Callback.h>
#include "../DebugContext.h"
#include "SpaceManager.h"

void MultiThreadSpace::Clear()
{
	_bufferBodies.clear();

	std::lock_guard lockMutex(_mutex);
	_bodies.clear();
}

void MultiThreadSpace::AddBody(const BodyData& body)
{
	std::lock_guard lockMutex(_mutex);
	_bodies.emplace_back(body);
}

void MultiThreadSpace::AddBodies(const std::vector<BodyData>& bodies)
{
	std::lock_guard lockMutex(_mutex);
	_bodies.append_range(bodies);
}

void MultiThreadSpace::Bodies(std::vector<BodyData>& bodies)
{
	std::lock_guard clockCopyBifferMutex(_bufferMutex);

	if (!_bufferBodies.empty()) {
		std::swap(bodies, _bufferBodies);
		_bufferBodies.clear();
	}
}

std::vector<BodyData> MultiThreadSpace::GetBodies()
{
	std::scoped_lock lockMutexes(_mutex, _bufferMutex);
	std::vector<BodyData> bodies;
	bodies.reserve(_bodies.size());

	for (const auto& body : _bodies) {
		bodies.emplace_back(body.mass, body.pos.x(), body.pos.y(), body.pos.z(), body.velocity.x(), body.velocity.y(), body.velocity.z());
	}

	return bodies;
}

float MultiThreadSpace::GetSubProgress() const
{
	return _subProcess.load();
}

float MultiThreadSpace::GetProgress() const
{
	return _process.load();
}

void MultiThreadSpace::Update()
{
	auto& debugContext = DebugContext::Instance();
	debugContext.countObject = _countObject.load();

	if (_isBusy.load() || SpaceManager::countOfIteration.load() == 0) {
		return;
	}

	debugContext.Clean();
	debugContext.deltaTime = SpaceManager::offsetIteration.load();

	_isBusy.store(true);
	std::thread th([this]() {	
		UpdateInternal();
		_isBusy.store(false);
		});

	th.detach();
}

void MultiThreadSpace::UpdateInternal()
{
	struct Colapce {
		int objectIndex = -1;
		mystd::Vec3 sumPos;
		mystd::Vec3 sumVelocity;
		float sumMass = 0.f;
	};

	static mystd::Vec3 noForce;
	_process.store(0.f);
	float iter = 0.f;
	const double beginTime = Engine::Callback::GetCurrentTime();

	while (iter < SpaceManager::countOfIteration.load()) {
		for (auto& body : _bodies) {
			body.force = noForce;
			body.colapseData = nullptr;
			body.radius = body.Radius();
		}

		const size_t count = _bodies.size();
		std::deque<Colapce> colapses;
		_subProcess.store(0.f);

		for (size_t i = 0; i < count; ++i) {
			for (size_t j = i + 1; j < count; ++j) {
				const auto direction = _bodies[j].pos - _bodies[i].pos;
				const float distance = direction.Length();

				if (distance <= (_bodies[i].radius + _bodies[j].radius)) {
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
				else {
					const float forceMagnitude = Space::constantGravity * _bodies[i].mass * _bodies[j].mass / std::pow(distance, 2);
					const auto forceDirection = direction.Normalized();
					const auto force = forceDirection * forceMagnitude;

					_bodies[i].force += force;
					_bodies[j].force -= force;
				}
			}

			_subProcess.store(static_cast<float>(i) / static_cast<float>(count));
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

		_bodies.erase(removeIt, _bodies.end());

		const float deltaTime = SpaceManager::offsetIteration.load();

		for (auto& body : _bodies) {
			const auto acceleration = body.force / body.mass;
			body.velocity += acceleration * deltaTime;
			body.pos += body.velocity * deltaTime;
		}

		_countObject.store(_bodies.size());

		{
			std::lock_guard lockCopyBufferMutex(_bufferMutex);
			_bufferBodies.clear();
			_bufferBodies.reserve(_bodies.size());

			for (const auto& body : _bodies) {
				_bufferBodies.emplace_back(body.mass, body.pos.x(), body.pos.y(), body.pos.z(), body.velocity.x(), body.velocity.y(), body.velocity.z());
			}
		}

		_process.store(iter / SpaceManager::countOfIteration.load());
		iter += 1.f;
	}

	DebugContext::Instance().updateDeltaTime.store((Engine::Callback::GetCurrentTime() - beginTime) / 1000);
}
