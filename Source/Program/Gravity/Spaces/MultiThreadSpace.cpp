// ◦ Xyz ◦

#include "MultiThreadSpace.h"
#include <thread>
#include <atomic>
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

void MultiThreadSpace::Update()
{
	auto& debugContext = DebugContext::Instance();
	debugContext.countObject = _countObject.load();

	if (_isBusy.load() || SpaceManager::countOfIteration.load() == 0) {
		return;
	}

	debugContext.Clean();
	debugContext.deltaTime = SpaceManager::offsetIteration.load();

	{
		std::lock_guard lockMutex(_mutex);
		_isBusy.store(true);
	}

	std::thread th([this]() {
		UpdateInternal();
		_isBusy.store(false);
		});

	LOG("MULTI [{}, {}] MAIN: {} SUB: {} countThread: {}", SpaceManager::paramA, SpaceManager::paramB, std::this_thread::get_id(), th.get_id(), std::thread::hardware_concurrency());
	th.detach();
}

void MultiThreadSpace::UpdateInternal()
{
	static mystd::Vec3 noForce;
	
	float iter = 0.f;

	std::lock_guard lockMutex(_mutex);
	const double beginTime = Engine::Callback::GetCurrentTime();
	_process.store(0.f);

	{
		for (auto& body : _bodies) {
			body.force = noForce;
			body.colapseData = nullptr;
			body.radius = body.Radius();
		}

		const size_t count = _bodies.size();
		std::deque<Colapce> colapses;
		const float dProgress = 1.f / static_cast<float>(count);
		_subProcess.store(0.f);

		if (!SpaceManager::paramA) {
			const size_t countThread = std::thread::hardware_concurrency();
			const size_t dSize = count / countThread;
			std::vector<std::pair<size_t, size_t>> ranges;
			ranges.emplace_back(0, dSize);

			for (size_t iTh = 1; iTh < countThread; ++iTh) {
				ranges.emplace_back(ranges.back().first + dSize, ranges.back().second + dSize);
			}
			ranges.back().second = count;

			if (!SpaceManager::paramB) {
				for (const auto& rengePair : ranges) {
					std::thread th([this, iBegin = rengePair.first, iEnd = rengePair.second, count, &colapses, dProgress]() {
						Iterations(iBegin, iEnd, count, colapses, dProgress);
						});
					LOG("   sub thread: {}", th.get_id());
					th.join();
				}
			}
			else {
				for (const auto& rengePair : ranges) {
					std::thread th([this, iBegin = rengePair.first, iEnd = rengePair.second, count, &colapses, dProgress]() {
						IterationsNoMutex(iBegin, iEnd, count, colapses, dProgress);
						});
					LOG("   sub thread: {} NO MUTEX", th.get_id());
					th.join();
				}
			}
		}
		else {
			IterationsNoMutex(0, count, count, colapses, dProgress);
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

	DebugContext::Instance().updateDeltaTime.store(Engine::Callback::GetCurrentTime() - beginTime);
}

void MultiThreadSpace::Iterations(size_t iBegin, size_t iEnd, size_t count, std::deque<Colapce>& colapses, float dProgress)
{
	for (size_t i = iBegin; i < iEnd; ++i) {
		for (size_t j = i + 1; j < count; ++j) {
			const auto direction = _bodies[j].pos - _bodies[i].pos;
			const float distance = direction.Length();

			if (distance <= (_bodies[i].radius + _bodies[j].radius)) {
				std::lock_guard lockIterationMutex(_iterationMutex);
				LOG("COLAPSE");

				Colapce* colapcePtr = _bodies[i].colapseData;
				if (!colapcePtr) {
					colapcePtr = _bodies[j].colapseData;
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

				std::lock_guard lockIterationMutex(_iterationMutex);
				_bodies[i].force += force;
				_bodies[j].force -= force;
			}
		}

		_subProcess.fetch_add(dProgress);
	}
}

void MultiThreadSpace::IterationsNoMutex(size_t iBegin, size_t iEnd, size_t count, std::deque<Colapce>& colapses, float dProgress)
{
	for (size_t i = iBegin; i < iEnd; ++i) {
		for (size_t j = i + 1; j < count; ++j) {
			const auto direction = _bodies[j].pos - _bodies[i].pos;
			const float distance = direction.Length();

			if (distance <= (_bodies[i].radius + _bodies[j].radius)) {
				std::lock_guard lockIterationMutex(_iterationMutex);
				LOG("COLAPSE");

				Colapce* colapcePtr = _bodies[i].colapseData;
				if (!colapcePtr) {
					colapcePtr = _bodies[j].colapseData;
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

		_subProcess.fetch_add(dProgress);
	}
}