// ◦ Xyz ◦

#include "GravitySpace.h"
#include <Common/Common.h>
#include <glm/ext/quaternion_geometric.hpp>
#include <deque>

Space::Space(std::string_view name)
	: Engine::Item(name)
{}

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
		
		object.weight = Engine::Random(0.1f, 10.f);
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
		float sumWeight = 0;
	};

	Object::gForce = 0.001f;
	float colapseDist = 0.1f;
	
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
					colapcePtr->sumWeight += _objects[i].weight;
					colapcePtr->objectIndex = i;
				}
				if (!_objects[j].colapseData) {
					_objects[j].colapseData = colapcePtr;
					colapcePtr->colapsCount += 1.f;
					colapcePtr->sumPos += _objects[j].pos;
					colapcePtr->sumWeight += _objects[j].weight;
				}
			}

			_objects[i].minDist = std::min(_objects[i].minDist, dist);
			_objects[j].minDist = std::min(_objects[j].minDist, dist);
		}
	}

	for (Colapce& colapses : colapses) {
		_objects[colapses.objectIndex].pos = colapses.sumPos / colapses.colapsCount;
		_objects[colapses.objectIndex].weight = colapses.sumWeight;
		_objects[colapses.objectIndex].colapseData = nullptr;
	}

	const auto removeIt = std::remove_if(_objects.begin(), _objects.end(), [](const auto& object) {
		float maxDist = 1000;
		if (object.minDist > maxDist) {
			LOG("REMOVE FORM DIST: {}", object.minDist);
		}
		return object.colapseData || object.minDist > maxDist;
		});

	size_t removed = std::distance(removeIt, _objects.end());
	_objects.erase(removeIt, _objects.end());
	if (removed != 0) {
		LOG("COLAPSE: {} => {}", removed, _objects.size());
	}

	colapses.clear();

	size = _objects.size();

	for (size_t i = 0; i < size; ++i) {
		auto& object = _objects[i];
		glm::vec3& pos = object.pos;
		glm::vec3& force = object.force;
		float& weight = object.weight;

		for (size_t j = 0; j < size; ++j) {
			if (i == j) {
				continue;
			}

			const float dist = glm::distance(pos, _objects[j].pos);
			const float valueForce = Object::gForce * weight * _objects[j].weight / std::powf(dist, 2);

			force += glm::normalize(_objects[j].pos - pos) * valueForce;
		}
	}
}

void Space::UpdateSpeed()
{
	float speed = 0.1f;
	const size_t size = _objects.size();

	for (size_t i = 0; i < size; ++i) {
		_objects[i].speed += _objects[i].force * speed;
	}
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
}

void Space::Update()
{
	UpdateForce();
	UpdateSpeed();
	UpdatePos();
}
