// ◦ Xyz ◦

#include "GravitySpace.h"
#include "../DebugContext.h"
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/vec3.hpp>
#include <deque>
#include <Common/Common.h>
#include "../../Temp/LogSpecification.h"
#include <Log.h>

void Space::Generate(size_t count, size_t radius)
{
	_objects.clear();
	_objects.reserve(count);

	for (int i = 0; i < count; ++i) {
		auto& object = _objects.emplace_back();
		
		object.pos.x = Engine::Random(-(float)radius, (float)radius);
		object.pos.y = Engine::Random(-(float)radius, (float)radius);
		object.pos.z = Engine::Random(-(float)radius, (float)radius);

		float speed = 0.005f;
		object.speed.x = Engine::Random(-(float)speed, (float)speed);
		object.speed.y = Engine::Random(-(float)speed, (float)speed);
		object.speed.z = Engine::Random(-(float)speed, (float)speed);
		
		object.mass = Engine::Random(0.1f, 10.f);
	}
}

const std::vector<Object>& Space::Objects() const
{
	return _objects;
}

void Space::UpdateForce()
{
	struct Colapce {
		int objectIndex = -1;
		float colapsCount = 0;
		glm::vec3 sumPos;
		glm::vec3 sumSpeed;
		float sumMass = 0;
	};

	DebugContext& debug = DebugContext::Instance();

	Object::gForce = 0.01f;
	debug.constForce = Object::gForce;

	float colapseDist = 2.f; // 1.f; // 0.1f;
	
	size_t size = _objects.size();

	std::deque<Colapce> colapses;

	for (size_t i = 0; i < size; ++i) {
		_objects[i].force.x = 0.f;
		_objects[i].force.y = 0.f;
		_objects[i].force.z = 0.f;
		_objects[i].colapseData = nullptr;

		for (size_t j = 0; j < size; ++j) {
			if (i == j) {
				continue;
			}

			const float dist = glm::distance(_objects[i].pos, _objects[j].pos);
			if (dist <= colapseDist) {
				Colapce* colapcePtr = static_cast<Colapce*>(_objects[i].colapseData);
				if (!colapcePtr) {
					colapcePtr = static_cast<Colapce*>(_objects[j].colapseData);
				}

				if (!colapcePtr) {
					colapcePtr = &colapses.emplace_back();
				}

				if (!_objects[i].colapseData) {
					_objects[i].colapseData = colapcePtr;
					colapcePtr->colapsCount += 1.f;
					colapcePtr->sumPos += _objects[i].pos;
					colapcePtr->sumSpeed += _objects[i].speed;
					colapcePtr->sumMass += _objects[i].mass;
					colapcePtr->objectIndex = i;
				}
				if (!_objects[j].colapseData) {
					_objects[j].colapseData = colapcePtr;
					colapcePtr->colapsCount += 1.f;
					colapcePtr->sumPos += _objects[j].pos;
					colapcePtr->sumSpeed += _objects[j].speed;
					colapcePtr->sumMass += _objects[j].mass;
				}
			}

			_objects[i].minDist = std::min(_objects[i].minDist, dist);
			_objects[j].minDist = std::min(_objects[j].minDist, dist);
		}
	}

	for (Colapce& colapses : colapses) {
		_objects[colapses.objectIndex].pos = colapses.sumPos / colapses.colapsCount;
		_objects[colapses.objectIndex].speed = colapses.sumSpeed / colapses.colapsCount;
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

	size = _objects.size();

	for (size_t i = 0; i < size; ++i) {
		auto& object = _objects[i];
		glm::vec3& pos = object.pos;
		glm::vec3& force = object.force;
		float& mass = object.mass;

		for (size_t j = 0; j < size; ++j) {
			if (i == j) {
				continue;
			}

			const float dist = glm::distance(pos, _objects[j].pos);
			const float valueForce = Object::gForce * mass * _objects[j].mass / std::powf(dist, 2);

			force += glm::normalize(_objects[j].pos - pos) * valueForce;
		}
	}

	DebugContext::Instance().countObject = _objects.size();
}

void Space::UpdateSpeed()
{
	DebugContext& debug = DebugContext::Instance();

	float speed = 0.1f;
	debug.constSpeed = speed;

	const size_t size = _objects.size();

	for (size_t i = 0; i < size; ++i) {
		float force = glm::length(_objects[i].force);
		debug.minForce = std::min(force, debug.minForce);
		debug.maxForce = std::max(debug.maxForce, force);		

		float mass = _objects[i].mass;
		debug.minMass = std::min(debug.minMass, mass);
		debug.maxMass = std::max(mass, debug.maxMass);

		_objects[i].speed += _objects[i].force * speed;
	}

	debug.middleForce = (debug.maxForce + debug.minForce) / 2.f;
	debug.middleMass = (debug.maxMass * debug.minMass) / 2.f;

	debug.minSpeed = debug.constSpeed * debug.minForce;
	debug.middleSpeed = debug.constSpeed * debug.middleForce;
	debug.maxSpeed = debug.constSpeed * debug.maxForce;
}

void Space::UpdatePos()
{
	const size_t size = _objects.size();

	for (size_t i = 0; i < size; ++i) {
		_objects[i].pos += _objects[i].speed;
	}
}

void Space::Clean()
{
	_objects.clear();
}

void Space::Update()
{
	DebugContext::Instance().Clean();
	UpdateForce();
	UpdateSpeed();
	UpdatePos();
}

glm::vec3 Space::PosOfMinSpeedObject() const
{
	float speed = std::numeric_limits<float>::max();
	const Object* obj = nullptr;

	for (size_t i = 0; i < _objects.size(); ++i) {
		if (glm::length(_objects[i].speed) < speed) {
			speed = glm::length(_objects[i].speed);
			obj = &_objects[i];
		}
	}

	if (!obj) {
		return {};
	}

	LOG("MIN SPEED: {} {} mass: {} pos: {}", glm::length(obj->speed), obj->speed, obj->mass, obj->pos);
	return obj->speed;
}

glm::vec3 Space::PosOfMaxSpeedObject() const
{
	float speed = 0.f;
	const Object* obj = nullptr;

	for (size_t i = 0; i < _objects.size(); ++i) {
		if (glm::length(_objects[i].speed) > speed) {
			speed = glm::length(_objects[i].speed);
			obj = &_objects[i];
		}
	}

	if (!obj) {
		return {};
	}
	
	LOG("MAX SPEED: {} {} mass: {} pos: {}", glm::length(obj->speed), obj->speed, obj->mass, obj->pos);
	return obj->speed;
}

glm::vec3 Space::PosOfMinMassObject() const
{
	glm::vec3 pos(0.f, 0.f, 0.f);
	float mass = std::numeric_limits<float>::max();
	const Object* obj = nullptr;

	for (size_t i = 0; i < _objects.size(); ++i) {
		if (_objects[i].mass < mass) {
			mass = _objects[i].mass;
			obj = &_objects[i];
		}
	}

	if (!obj) {
		return {};
	}

	LOG("MIN MASS: {}  speed: {} {} pos: {}", obj->mass, glm::length(obj->speed), obj->speed, obj->pos);
	return obj->pos;
}

glm::vec3 Space::PosOfMaxMassObject() const
{
	const Object* obj = nullptr;
	float mass = 0.f;

	for (size_t i = 0; i < _objects.size(); ++i) {
		if (_objects[i].mass > mass) {
			mass = _objects[i].mass;
			obj = &_objects[i];
		}
	}

	if (!obj) {
		return {};
	}

	LOG("MAX MASS: {}  speed: {} {} pos: {}", obj->mass, glm::length(obj->speed), obj->speed, obj->pos);
	return obj->pos;
}