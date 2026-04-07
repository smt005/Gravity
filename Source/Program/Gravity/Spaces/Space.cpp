// ◦ Xyz ◦
#include "Space.h"

using namespace Spaces;

const std::string& Space::GetName() const
{
	if (_name.empty()) _name = Engine::ExtractClassName(typeid(*this).name());
	return _name;
}

size_t Space::GetId() const {
	if (!_id) _id = typeid(*this).hash_code();
	return _id;
}

float Space::Radius(float mass)
{
	return std::cbrt((3.f * mass) / (4.f * std::numbers::pi));
}

float Space::Diameter(float mass)
{
	const float radius = std::cbrt((3.f * mass) / (4.f * std::numbers::pi));
	return radius * 2.f;
}
