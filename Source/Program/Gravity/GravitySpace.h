// ◦ Xyz ◦
#pragma once

#include <vector>
#include <Common/Item.h>
#include <glm/vec3.hpp>
#include "Object.h"

class Space final : public Engine::Item {
public:
	using Ptr = std::shared_ptr<Space>;

	Space(std::string_view name);

	void Generate(size_t count, size_t radius);
	const std::vector<Object>& Objects() const;
	void UpdateForce();
	void UpdatePos();
	void Update();

private:
	std::vector<Object> _objects;
};
