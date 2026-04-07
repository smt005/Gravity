// ◦ Xyz ◦
#include "Space.h"

using namespace Spaces;

const std::string& Space::GetName() const
{
	if (_name.empty()) _name = Engine::ExtractClassName(typeid(*this).name());
	return _name;
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
