// ◦ Xyz ◦

#include "MultiAllThreadSpace.h"
#include <thread>
#include <atomic>
#include <glm/gtc/quaternion.hpp>
#include <Callback/Callback.h>
#include "../DebugContext.h"
#include "SpaceManager.h"

void MultiAllThreadSpace::Clear()
{
	std::unique_lock lockMutex(_mutex);
	_conditionMutex.wait(lockMutex, [this]() { return _countBusy.load() == 0; });
	
	_bufferBodies.clear();
	_bodies.clear();
}

void MultiAllThreadSpace::AddBody(const BodyData& body)
{
	std::unique_lock lockMutex(_mutex);
	_conditionMutex.wait(lockMutex, [this]() { return _countBusy.load() == 0; });

	_bodies.emplace_back(body);
}

void MultiAllThreadSpace::AddBodies(const std::vector<BodyData>& bodies)
{
	std::unique_lock lockMutex(_mutex);
	_conditionMutex.wait(lockMutex, [this]() { return _countBusy.load() == 0; });

	_bodies.append_range(bodies);
}

void MultiAllThreadSpace::Bodies(std::vector<BodyData>& bodies)
{
	std::lock_guard clockCopyBifferMutex(_bufferMutex);

	if (!_bufferBodies.empty()) {
		std::swap(bodies, _bufferBodies);
		_bufferBodies.clear();
	}
}

std::vector<BodyData> MultiAllThreadSpace::GetBodies()
{
	std::unique_lock lockMutex(_mutex);
	_conditionMutex.wait(lockMutex, [this]() { return _countBusy.load() == 0; });

	std::vector<BodyData> bodies;
	bodies.reserve(_bodies.size());

	for (const auto& body : _bodies) {
		bodies.emplace_back(body.mass, body.pos.x(), body.pos.y(), body.pos.z(), body.velocity.x(), body.velocity.y(), body.velocity.z());
	}

	return bodies;
}

void MultiAllThreadSpace::Update()
{
	auto& debugContext = DebugContext::Instance();
	debugContext.countObject = _countObject.load();

	if (_countBusy.load() || SpaceManager::countOfIteration.load() == 0) {
		return;
	}

	_countBusy.fetch_add(1);
	debugContext.Clean();
	debugContext.deltaTime = SpaceManager::offsetIteration.load();

	UpdateInternal();
}

void MultiAllThreadSpace::UpdateInternal()
{
	static mystd::Vec3 noForce;

	std::lock_guard lockMutex(_mutex);
	_beginTime = Engine::Callback::GetCurrentTime();
	_process.store(0.f);

	for (auto& body : _bodies) {
		body.force = noForce;
		body.colapseData = nullptr;
		body.radius = body.Radius();
	}

	const size_t count = _bodies.size();
	_colapses.clear();
	_subProcess.store(0.f);

	const size_t countThread = std::thread::hardware_concurrency();
	const size_t dSize = count / countThread;
	std::vector<std::pair<size_t, size_t>> ranges;
	ranges.emplace_back(0, dSize);

	for (size_t iTh = 1; iTh < countThread; ++iTh) {
		ranges.emplace_back(ranges.back().first + dSize, ranges.back().second + dSize);
	}
	ranges.back().second = count;

	const float dProgress = 1.f / static_cast<float>(count);

	for (const auto& rengePair : ranges) {
		_countBusy.fetch_add(1);
	
		std::thread th([this, iBegin = rengePair.first, iEnd = rengePair.second, count, dProgress]() {
			if (!SpaceManager::paramB) {
				Iterations(iBegin, iEnd, count, _colapses, dProgress);
			}
			else {
				IterationsNoMutex(iBegin, iEnd, count, _colapses, dProgress);
			}
			_countBusy.fetch_sub(1);

			if (_countBusy.load() == 1) {
				ColapseBodies();
			}
		});

		if (SpaceManager::paramC) LOG("MULTI ALL [{}, {}] MAIN: {} SUB: {} countThread: {}", SpaceManager::paramA, SpaceManager::paramB, std::this_thread::get_id(), th.get_id(), std::thread::hardware_concurrency());
		th.detach();
	}
}

void MultiAllThreadSpace::ColapseBodies()
{
	for (Colapce& colapses : _colapses) {
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
		std::scoped_lock lockCopyBufferMutex(_mutex, _bufferMutex);
		_bufferBodies.clear();
		_bufferBodies.reserve(_bodies.size());

		for (const auto& body : _bodies) {
			_bufferBodies.emplace_back(body.mass, body.pos.x(), body.pos.y(), body.pos.z(), body.velocity.x(), body.velocity.y(), body.velocity.z());
		}
	}

	DebugContext::Instance().updateDeltaTime.store(Engine::Callback::GetCurrentTime() - _beginTime);

	_countBusy.fetch_sub(1);
	_conditionMutex.notify_one();
}

void MultiAllThreadSpace::Iterations(size_t iBegin, size_t iEnd, size_t count, std::deque<Colapce>& colapses, float dProgress)
{
	for (size_t i = iBegin; i < iEnd; ++i) {
		for (size_t j = i + 1; j < count; ++j) {
			const auto direction = _bodies[j].pos - _bodies[i].pos;
			const float distance = direction.Length();

			if (distance <= (_bodies[i].radius + _bodies[j].radius)) {
				std::lock_guard lockIterationMutex(_iterationMutex);
				if (SpaceManager::paramC) LOG("COLAPSE");

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

void MultiAllThreadSpace::IterationsNoMutex(size_t iBegin, size_t iEnd, size_t count, std::deque<Colapce>& colapses, float dProgress)
{
	for (size_t i = iBegin; i < iEnd; ++i) {
		for (size_t j = i + 1; j < count; ++j) {
			const auto direction = _bodies[j].pos - _bodies[i].pos;
			const float distance = direction.Length();

			if (distance <= (_bodies[i].radius + _bodies[j].radius)) {
				std::lock_guard lockIterationMutex(_iterationMutex);
				if (SpaceManager::paramC) LOG("COLAPSE");

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