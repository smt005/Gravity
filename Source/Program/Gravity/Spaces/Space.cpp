// ◦ Xyz ◦

#include "Space.h"
#include <deque>
#include <numbers>
#include <glm/vec3.hpp>
#include <glm/ext/vector_common.hpp>
#include <glm/gtc/quaternion.hpp>
#include <Common/Common.h>
#include "../DebugContext.h"
#include "SpaceManager.h"
#include "../../Temp/LogSpecification.h"
#include <Log.h>

#define _USE_MATH_DEFINE

Space& Space::operator = (const Space& in)
{
	int typeTemp = in.typeTemp;
	int countTemp = in.countTemp;
	int sizeTemp = in.sizeTemp;

	_objects = in._objects;
	return *this;
}

Space& Space::operator = (const std::vector<Object>& objects)
{
	_objects = objects;
	return *this;
}

void Space::Generate(size_t count, size_t radius, int typeGenerate)
{
	typeTemp = typeGenerate;
	countTemp = count;
	sizeTemp = radius;

	_objects.clear();
	_objects.reserve(count);
	glm::vec3 vec3(1.f, 0.f, 0.f);

	for (int i = 0; i < count; ++i) {
		auto& object = _objects.emplace_back();

		if (typeGenerate == 1) {
			float angle = Engine::Random<double>(0, std::numbers::pi * 2);

			glm::vec3 objectVec3(0.f, 0.f, 0.f);
			objectVec3.x = vec3.x * cos(angle) - vec3.y * sin(angle);
			objectVec3.y = vec3.x * sin(angle) + vec3.y * cos(angle);
			objectVec3 *= Engine::Random(0.f, (float)radius);

			object.pos = objectVec3;
			object.mass = Engine::Random(0.1f, 1.f);
		}
		else {
			object.pos.x = Engine::Random(-(float)radius, (float)radius);
			object.pos.y = Engine::Random(-(float)radius, (float)radius);
			object.pos.z = Engine::Random(-(float)radius, (float)radius);

			float speed = 0.005f;
			object.velocity.x = Engine::Random(-(float)speed, (float)speed);
			object.velocity.y = Engine::Random(-(float)speed, (float)speed);
			object.velocity.z = Engine::Random(-(float)speed, (float)speed);

			object.mass = Engine::Random(0.1f, 1.f);
		}
	}
}

const std::vector<Object>& Space::Objects() const
{
	return _objects;
}

const std::vector<Body>& Space::Bodies() const
{
	return _objects;
}

void Space::Clean()
{
	_objects.clear();
}

void Space::Update() {
	if (SpaceManager::countOfIteration == 0) {
		return;
	}

	const double countObject = static_cast<double>(_objects.size());
	if (!countObject) {
		return;
	}

	static float angleSpeedFactor = 0.001f;
	float angleSpeed = static_cast<float>(SpaceManager::offsetIteration) * static_cast<float>(SpaceManager::countOfIteration) * angleSpeedFactor;
	glm::vec3 rotationAxis(0.f, 0.f, 1.f);
	glm::quat rotationQuat = glm::angleAxis(angleSpeed, rotationAxis);

	int completeObjects = 0;

	for (auto& object : _objects) {
		object.pos = rotationQuat * object.pos;

		++completeObjects;
		progress = (float)completeObjects / (float)countObject;
	}
}

void Space::CollectDebugData() const
{
	DebugContext& debugContext = DebugContext::Instance();

	auto& _bodies = _objects; // TODO:

	for (auto& body : _bodies) {
		debugContext.minForce = std::min(debugContext.minForce, glm::length(body.force));
		debugContext.minMass = std::min(debugContext.minMass, glm::length(body.mass));
		debugContext.minVelocity = std::min(debugContext.minVelocity, glm::length(body.velocity));
		debugContext.maxForce = std::max(debugContext.maxForce, glm::length(body.force));
		debugContext.maxMass = std::max(debugContext.maxMass, glm::length(body.mass));
		debugContext.maxVelocity = std::max(debugContext.maxVelocity, glm::length(body.velocity));
		debugContext.sumMass += body.mass;
	}
}

void Space::SwitchPause()
{
	_pause = !_pause;
}

const std::string& Space::GetName() const
{
	if (_name.empty()) {
		_name = Engine::ExtractClassName(typeid(*this).name());
	}
	return _name;
}

size_t Space::GetId() const
{
	if (!_id) {
		_id = typeid(*this).hash_code();
	}
	return _id;
}

bool Space::IsPaused() const
{
	return _pause;
}

glm::vec3 Space::PosOfMinSpeedObject() const
{
	float speed = std::numeric_limits<float>::max();
	const Object* obj = nullptr;

	for (size_t i = 0; i < _objects.size(); ++i) {
		if (glm::length(_objects[i].velocity) < speed) {
			speed = glm::length(_objects[i].velocity);
			obj = &_objects[i];
		}
	}

	if (!obj) {
		return {};
	}

	LOG("MIN SPEED: {} {} mass: {} pos: {}", glm::length(obj->velocity), obj->velocity, obj->mass, obj->pos);
	return obj->velocity;
}

glm::vec3 Space::PosOfMaxSpeedObject() const
{
	float velocity = 0.f;
	const Object* obj = nullptr;

	for (size_t i = 0; i < _objects.size(); ++i) {
		if (glm::length(_objects[i].velocity) > velocity) {
			velocity = glm::length(_objects[i].velocity);
			obj = &_objects[i];
		}
	}

	if (!obj) {
		return {};
	}
	
	LOG("MAX SPEED: {} {} mass: {} pos: {}", glm::length(obj->velocity), obj->velocity, obj->mass, obj->pos);
	return obj->velocity;
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

	LOG("MIN MASS: {}  velocity: {} {} pos: {}", obj->mass, glm::length(obj->velocity), obj->velocity, obj->pos);
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

	LOG("MAX MASS: {}  velocity: {} {} pos: {}", obj->mass, glm::length(obj->velocity), obj->velocity, obj->pos);
	return obj->pos;
}