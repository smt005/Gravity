// ◦ Xyz ◦

#include "GravitySpace.h"
#include <Common/Common.h>
#include <glm/ext/quaternion_geometric.hpp>

Space::Space(std::string_view name)
	: Engine::Item(name)
{}

void Space::Generate(size_t count, size_t radius)
{
	_objects.clear();
	_objects.reserve(count);

	for (int i = 0; i < count; ++i) {
		float factor = 1000;
		auto& object = _objects.emplace_back();
		
		object.pos.x = Engine::Random(-(float)radius, (float)radius);
		object.pos.y = Engine::Random(-(float)radius, (float)radius);
		object.pos.z = Engine::Random(-(float)radius, (float)radius);

		float force = 0.01f;
		object.force.x = Engine::Random(-(float)force, (float)force);
		object.force.y = Engine::Random(-(float)force, (float)force);
		object.force.z = Engine::Random(-(float)force, (float)force);
	}
}

const std::vector<Object>& Space::Objects() const
{
	return _objects;
}

void Space::UpdateForce()
{
	const size_t size = _objects.size();

	for (size_t i = 0; i < size; ++i) {
		_objects[i].force.x = 0.f;
		_objects[i].force.y = 0.f;
		_objects[i].force.z = 0.f;
	}

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
			_objects[j].force += glm::normalize(pos - _objects[j].pos) * valueForce;
		}
	}
}

void Space::UpdatePos()
{
	float speed = 1.f;
	const size_t size = _objects.size();

	for (size_t i = 0; i < size; ++i) {
		_objects[i].pos += _objects[i].force * speed;
	}
}

void Space::Update()
{
	UpdateForce();
	UpdatePos();
}
