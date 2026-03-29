// ◦ Xyz ◦

#include "MainThreadSpace.h"
#include <deque>
#include <atomic>
#include <Callback/Callback.h>
#include "../DebugContext.h"
#include "SpaceManager.h"

void MainThreadSpace::Clear()
{
	_bodies.clear();
}

void MainThreadSpace::AddBody(const BodyData& body)
{
	_bodies.emplace_back(body);
}

void MainThreadSpace::AddBodies(const std::vector<BodyData>& bodies)
{
	_bodies.append_range(bodies);
}

void MainThreadSpace::Bodies(std::vector<BodyData>& bodies)
{
	bodies.clear();
	bodies.reserve(_bodies.size());

	for (const auto& body : _bodies) {
		bodies.emplace_back(body.mass, body.pos.x(), body.pos.y(), body.pos.z(), body.velocity.x(), body.velocity.y(), body.velocity.z());
	}
}

std::vector<BodyData> MainThreadSpace::GetBodies()
{
	std::vector<BodyData> bodies;
	bodies.reserve(_bodies.size());

	for (const auto& body : _bodies) {
		bodies.emplace_back(body.mass, body.pos.x(), body.pos.y(), body.pos.z(), body.velocity.x(), body.velocity.y(), body.velocity.z());
	}

	return bodies;
}

void MainThreadSpace::Update()
{
	if (SpaceManager::countOfIteration == 0) {
		return;
	}

	auto& debugContext = DebugContext::Instance();
	debugContext.Clean();

	UpdateInternal();

	debugContext.deltaTime = SpaceManager::offsetIteration.load();
	debugContext.countObject = _bodies.size();
}

void MainThreadSpace::UpdateInternal()
{
	struct Colapce {
		int objectIndex = -1;
		mystd::Vec3 sumPos;
		mystd::Vec3 sumVelocity;
		float sumMass = 0.f;
	};

	static mystd::Vec3 noForce;
	_process.store(0.f);
	float deltaTime = SpaceManager::offsetIteration.load();
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

		for (auto& body : _bodies) {
			const auto acceleration = body.force / body.mass;
			body.velocity += acceleration * deltaTime;
			body.pos += body.velocity * deltaTime;
		}

		_process.store(iter / SpaceManager::countOfIteration.load());
		iter += 1.f;
	}

	DebugContext::Instance().updateDeltaTime.store(Engine::Callback::GetCurrentTime() - beginTime);
}
