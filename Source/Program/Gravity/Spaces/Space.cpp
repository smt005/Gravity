// ◦ Xyz ◦

#include "Space.h"
#include <deque>
#include <numbers>
#include <glm/vec3.hpp>
#include <glm/ext/vector_common.hpp>
#include <Common/Common.h>
#include "../DebugContext.h"
#include "../../Temp/LogSpecification.h"
#include <Log.h>

#define _USE_MATH_DEFINE

Space::Space()
{
	SetName("Space");
}

void Space::Generate(size_t count, size_t radius)
{
	_objects.clear();
	_objects.reserve(count);
	glm::vec3 vec3(1.f, 0.f, 0.f);
	int typeGenerate = 1;

	for (int i = 0; i < count; ++i) {
		auto& object = _objects.emplace_back();
		
		if (typeGenerate == 0) {
			object.pos.x = Engine::Random(-(float)radius, (float)radius);
			object.pos.y = Engine::Random(-(float)radius, (float)radius);
			object.pos.z = Engine::Random(-(float)radius, (float)radius);

			float speed = 0.005f;
			object.speed.x = Engine::Random(-(float)speed, (float)speed);
			object.speed.y = Engine::Random(-(float)speed, (float)speed);
			object.speed.z = Engine::Random(-(float)speed, (float)speed);

			object.mass = Engine::Random(0.1f, 1.f);
		}
		else if (typeGenerate == 1) {
			float angle = Engine::Random<double>(0, std::numbers::pi * 2);

			glm::vec3 objectVec3(0.f, 0.f, 0.f);
			objectVec3.x = vec3.x * cos(angle) - vec3.y * sin(angle);
			objectVec3.y = vec3.x * sin(angle) + vec3.y * cos(angle);
			//objectVec3.z = Engine::Random(-0.1f, 0.1f);
			objectVec3 *= Engine::Random(0.f, (float)radius);

			object.pos = objectVec3;
			object.mass = Engine::Random(0.1f, 1.f);
		}
	}
}

const std::vector<Object>& Space::Objects() const
{
	return _objects;
}

void Space::Clean()
{
	_objects.clear();
}

void Space::Update() {}

void Space::SwitchPause()
{
	_pause = !_pause;
}

void Space::SetName(std::string name)
{
	_name = std::string(name);
}

const std::string& Space::GetName() const
{
	return _name;
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