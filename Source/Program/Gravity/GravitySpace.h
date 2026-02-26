// ◦ Xyz ◦
#pragma once

#include <vector>
#include <Common/Item.h>
#include <glm/vec3.hpp>

class Space final : public Engine::Item {
public:
	using Ptr = std::shared_ptr<Space>;

	Space(const std::string& name);

	void Generate(size_t count);

private:
public:
	std::vector<glm::vec3> _data;
};
