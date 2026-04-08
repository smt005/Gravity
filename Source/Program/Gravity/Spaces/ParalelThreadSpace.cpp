// ◦ Xyz ◦

#include "ParalelThreadSpace.h"
#include <atomic>
#include "../DebugContext.h"
#include "SpaceManager.h"

ParalelThread::ParalelThread()
{
	DebugContext::Instance().deltaTimes.resize(1);
}

void ParalelThread::Clear()
{
	std::lock_guard lockMutex(_mutex);
	_bodies.clear();
}


void ParalelThread::AddBody(const BodyData& body)
{
	std::lock_guard lockMutex(_mutex);
	_bodies.emplace_back(body);
}

void ParalelThread::AddBodies(const std::vector<BodyData>& bodies)
{
	std::lock_guard lockMutex(_mutex);
	_bodies.append_range(bodies);
}

void ParalelThread::Bodies(std::vector<GravityRender::Body>& bodies)
{
	if (_renderBodies.empty()) {
		return;
	}

	std::lock_guard lockMutex(_copyMutex);

	std::swap(bodies, _renderBodies);
	_renderBodies.clear();
}

std::vector<BodyData> ParalelThread::GetBodies()
{
	std::vector<BodyData> bodies;
	bodies.reserve(_bodies.size());

	{
		std::lock_guard lockMutex(_mutex);
		for (const Body& body : _bodies) {
			const auto& pos = body.pos;
			const auto& vel = body.velocity;
			bodies.emplace_back(body.mass, pos.X(), pos.Y(), pos.Z(), vel.X(), vel.Y(), vel.Z());
		}
	}

	return bodies;
}

void ParalelThread::Update()
{
	if (_processing.load()) {
		return;
	}

	{
		std::lock_guard lockMutex(_mutex);
		if (_processing.load()) {
			return;
		}
		_processing.store(true);
	}

	std::thread th([this]() {
		std::lock_guard lockMutex(_mutex);
		for (auto& body : _bodies) {
			body.force = { 0.f, 0.f, 0.f };
		}
		const size_t size = _bodies.size();
		std::deque<Colapce> colapses;
		std::vector<Colapce*> colapseOfBodies(size, nullptr);
		
		UpdateForce(0, size, size, colapses, colapseOfBodies);
		ColapceBodies(colapses, colapseOfBodies);
		UpdatePositions();
		CopyBodies();

		_processing.store(false);
	});

	th.detach();
}

void ParalelThread::UpdateForce(size_t iBegin, size_t iEnd, size_t size, std::deque<Colapce>& colapses, std::vector<Colapce*>& colapseOfBodies)
{
	Engine::TimeHundler timer;

	for (size_t i = iBegin; i < iEnd; ++i) {
		auto& bodyForce = _bodies[i].force;

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
				const auto force = forceDirection * forceMagnitude;
				bodyForce += force;
				_bodies[j].force -= force;
			}
		}
	}

	DebugContext::Instance().deltaTimes[0] = timer.GetDeltaTime();
}

void ParalelThread::ColapceBodies(std::deque<Colapce>& colapses, std::vector<Colapce*>& colapseOfBodies)
{
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

void ParalelThread::UpdatePositions()
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

void ParalelThread::CopyBodies()
{
	std::lock_guard lockMutex(_copyMutex);

	_renderBodies.clear();
	_renderBodies.resize(_bodies.size());

	std::transform(_bodies.begin(), _bodies.end(), _renderBodies.begin(), [](const Body& body) {
		return GravityRender::Body{ Diameter(body.mass), { body.pos.X(), body.pos.Y(), body.pos.Z()} };
	});
}
