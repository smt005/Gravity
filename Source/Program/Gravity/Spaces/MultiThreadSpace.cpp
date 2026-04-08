// ◦ Xyz ◦

#include "MultiThreadSpace.h"
#include <atomic>
#include "../DebugContext.h"
#include "SpaceManager.h"

MultiThread::MultiThread()
{
	DebugContext::Instance().deltaTimes.resize(1);
	params.emplace_back(false, "double set force");
	params.emplace_back(false, "disable mutex");
}

void MultiThread::Clear()
{
	std::unique_lock lockMutex(_mutex);
	_conditionMutex.wait(lockMutex, [this]() { return _countProcessing.load() == 0; });
	_bodies.clear();
}

void MultiThread::AddBody(const BodyData& body)
{
	std::unique_lock lockMutex(_mutex);
	_conditionMutex.wait(lockMutex, [this]() { return _countProcessing.load() == 0; });
	_bodies.emplace_back(body);
}

void MultiThread::AddBodies(const std::vector<BodyData>& bodies)
{
	std::unique_lock lockMutex(_mutex);
	_conditionMutex.wait(lockMutex, [this]() { return _countProcessing.load() == 0; });
	_bodies.append_range(bodies);
}

void MultiThread::Bodies(std::vector<GravityRender::Body>& bodies)
{
	if (_renderBodies.empty()) {
		return;
	}

	std::lock_guard lockMutex(_copyMutex);

	std::swap(bodies, _renderBodies);
	_renderBodies.clear();
}

std::vector<BodyData> MultiThread::GetBodies()
{
	std::vector<BodyData> bodies;
	bodies.reserve(_bodies.size());

	{
		std::unique_lock lockMutex(_mutex);
		_conditionMutex.wait(lockMutex, [this]() { return _countProcessing.load() == 0; });

		for (const Body& body : _bodies) {
			const auto& pos = body.pos;
			const auto& vel = body.velocity;
			bodies.emplace_back(body.mass, pos.X(), pos.Y(), pos.Z(), vel.X(), vel.Y(), vel.Z());
		}
	}

	return bodies;
}


void MultiThread::Update()
{
	if (_countProcessing.load() != 0) {
		return;
	}

	{
		std::lock_guard lockMutex(_mutex);
		if (_countProcessing.load() != 0) {
			return;
		}
		_countProcessing.fetch_add(1);
	}

	std::lock_guard lockMutex(_mutex);

	for (auto& body : _bodies) {
		body.force = { 0.f, 0.f, 0.f };
	}
	const size_t size = _bodies.size();
	_colapses.clear();
	_colapseOfBodies.clear();
	_colapseOfBodies.resize(size, nullptr);
	
	std::vector<std::pair<size_t, size_t>> ranges;
	{
		const size_t hardwareConcurrency = std::thread::hardware_concurrency();
		const size_t countThread = size > (hardwareConcurrency * 100) ? hardwareConcurrency : 1;
		const size_t dSize = size / countThread;
		ranges.emplace_back(0, dSize);

		for (size_t iTh = 1; iTh < countThread; ++iTh) {
			ranges.emplace_back(ranges.back().first + dSize, ranges.back().second + dSize);
		}
		ranges.back().second = size;
	}

	const bool typeProcecc = params[0].first;
	const bool disableMutex = params[1].first;
	_beginTime = Engine::Callback::GetCurrentTime();

	for (const auto& rengePair : ranges) {
		_countProcessing.fetch_add(1);

		std::thread th([this, iBegin = rengePair.first, iEnd = rengePair.second, size, typeProcecc, disableMutex]() {
			if (typeProcecc) {
				if (disableMutex) {
					UpdateForceHalfNoMutex(iBegin, iEnd, size);
				}
				else {
					UpdateForceHalf(iBegin, iEnd, size);
				}
			}
			else {
				UpdateForce(iBegin, iEnd, size);
			}

			_countProcessing.fetch_sub(1);
			if (_countProcessing.load() == 1) {
				DebugContext::Instance().deltaTimes[0] = Engine::Callback::GetCurrentTime() - _beginTime;
				ApplyForce();
			}
		});

		th.detach();
	}
}

void MultiThread::UpdateForce(size_t iBegin, size_t iEnd, size_t size)
{
	for (size_t i = iBegin; i < iEnd; ++i) {
		auto& bodyForce = _bodies[i].force;

		for (size_t j = 0; j < size; ++j) {
			if (i == j) {
				continue;
			}

			const auto direction = _bodies[j].pos - _bodies[i].pos;
			const float distance = direction.Length();

			if (distance <= (_bodies[i].radius + _bodies[j].radius)) {
				std::lock_guard lockMutex(_copyMutex);

				Colapce* colapcePtr = _colapseOfBodies[i];
				if (!colapcePtr) {
					colapcePtr = _colapseOfBodies[j];
				}
				if (!colapcePtr) {
					colapcePtr = &_colapses.emplace_back(i);
				}
				if (!_colapseOfBodies[i]) {
					_colapseOfBodies[i] = colapcePtr;
					const auto& body = _bodies[i];
					colapcePtr->sumPos += body.pos * body.mass;
					colapcePtr->sumMass += body.mass;
					colapcePtr->sumVelocity += body.velocity * body.mass;
				}
				if (!_colapseOfBodies[j]) {
					_colapseOfBodies[j] = colapcePtr;
					const auto& body = _bodies[j];
					colapcePtr->sumPos += body.pos * body.mass;
					colapcePtr->sumMass += body.mass;
					colapcePtr->sumVelocity += body.velocity * body.mass;
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

void MultiThread::UpdateForceHalf(size_t iBegin, size_t iEnd, size_t size)
{
	for (size_t i = iBegin; i < iEnd; ++i) {
		auto& bodyForce = _bodies[i].force;

		for (size_t j = i + 1; j < size; ++j) {
			const auto direction = _bodies[j].pos - _bodies[i].pos;
			const float distance = direction.Length();

			if (distance <= (_bodies[i].radius + _bodies[j].radius)) {
				std::lock_guard lockMutex(_copyMutex);

				Colapce* colapcePtr = _colapseOfBodies[i];
				if (!colapcePtr) {
					colapcePtr = _colapseOfBodies[j];
				}
				if (!colapcePtr) {
					colapcePtr = &_colapses.emplace_back(i);
				}
				if (!_colapseOfBodies[i]) {
					_colapseOfBodies[i] = colapcePtr;
					const auto& body = _bodies[i];
					colapcePtr->sumPos += body.pos * body.mass;
					colapcePtr->sumMass += body.mass;
					colapcePtr->sumVelocity += body.velocity * body.mass;
				}
				if (!_colapseOfBodies[j]) {
					_colapseOfBodies[j] = colapcePtr;
					const auto& body = _bodies[j];
					colapcePtr->sumPos += body.pos * body.mass;
					colapcePtr->sumMass += body.mass;
					colapcePtr->sumVelocity += body.velocity * body.mass;
				}
			}
			else {
				const float forceMagnitude = Space::constantGravity * _bodies[i].mass * _bodies[j].mass / std::pow(distance, 2);
				const auto forceDirection = direction.Normalized();
				const auto force = forceDirection * forceMagnitude;

				std::lock_guard lockMutex(_copyMutex);
				bodyForce += force;
				_bodies[j].force -= force;
			}
		}
	}
}

void MultiThread::UpdateForceHalfNoMutex(size_t iBegin, size_t iEnd, size_t size)
{
	for (size_t i = iBegin; i < iEnd; ++i) {
		auto& bodyForce = _bodies[i].force;

		for (size_t j = i + 1; j < size; ++j) {
			const auto direction = _bodies[j].pos - _bodies[i].pos;
			const float distance = direction.Length();

			if (distance <= (_bodies[i].radius + _bodies[j].radius)) {
				std::lock_guard lockMutex(_copyMutex);

				Colapce* colapcePtr = _colapseOfBodies[i];
				if (!colapcePtr) {
					colapcePtr = _colapseOfBodies[j];
				}
				if (!colapcePtr) {
					colapcePtr = &_colapses.emplace_back(i);
				}
				if (!_colapseOfBodies[i]) {
					_colapseOfBodies[i] = colapcePtr;
					const auto& body = _bodies[i];
					colapcePtr->sumPos += body.pos * body.mass;
					colapcePtr->sumMass += body.mass;
					colapcePtr->sumVelocity += body.velocity * body.mass;
				}
				if (!_colapseOfBodies[j]) {
					_colapseOfBodies[j] = colapcePtr;
					const auto& body = _bodies[j];
					colapcePtr->sumPos += body.pos * body.mass;
					colapcePtr->sumMass += body.mass;
					colapcePtr->sumVelocity += body.velocity * body.mass;
				}
			}
			else {
				const float forceMagnitude = Space::constantGravity * _bodies[i].mass * _bodies[j].mass / std::pow(distance, 2);
				const auto forceDirection = direction.Normalized();
				const auto force = forceDirection * forceMagnitude;
				bodyForce += force;
				_bodies[j].force -= force;
			}
		}
	}
}

void MultiThread::ColapceBodies()
{
	for (Colapce& colapse : _colapses) {
		auto& body = _bodies[colapse.objectIndex];
		body.mass = colapse.sumMass;
		body.pos = colapse.sumPos / colapse.sumMass;
		body.radius = Diameter(body.mass);
		body.velocity = colapse.sumVelocity / colapse.sumMass;
		_colapseOfBodies[colapse.objectIndex] = nullptr;
	}

	size_t size = _bodies.size();
	size_t iLast = size - 1;
	size_t i = 0;

	while (i < iLast) {
		if (_colapseOfBodies[i]) {
			_bodies[_colapseOfBodies[i]->objectIndex].force += _bodies[i].force;
			_colapseOfBodies[i] = _colapseOfBodies[iLast];
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

void MultiThread::UpdatePositions()
{
	volatile static float velocityFactor = 1.f;
	const float deltaTime = SpaceManager::offsetIteration.load() * velocityFactor;
	const size_t size = _bodies.size();

	for (auto& body : _bodies) {
		const auto acceleration = body.force / body.mass;
		body.velocity += acceleration * deltaTime;
		body.pos += body.velocity * deltaTime;
	}
}

void MultiThread::CopyBodies()
{
	std::lock_guard lockMutex(_copyMutex);

	_renderBodies.clear();
	_renderBodies.resize(_bodies.size());

	std::transform(_bodies.begin(), _bodies.end(), _renderBodies.begin(), [](const Body& body) {
		return GravityRender::Body{ Diameter(body.mass), { body.pos.X(), body.pos.Y(), body.pos.Z()} };
		});
}


void MultiThread::ApplyForce()
{
	ColapceBodies();
	UpdatePositions();
	CopyBodies();

	_countProcessing.fetch_sub(1);
	_conditionMutex.notify_one();
}
