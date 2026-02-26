// ◦ Xyz ◦

#include "GravitySpace.h"

Space::Space(const std::string& name)
	: Engine::Item(name)
{}

void Space::Generate(size_t count)
{
	_data.clear();
	_data.resize(count);
}
