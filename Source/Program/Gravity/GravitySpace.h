// ◦ Xyz ◦
#pragma once

#include <vector>
#include "Singletone.h"
#include "Object.h"

class Space final : public mystd::Singletone<Space> {
public:
	void Generate(size_t count, size_t radius);
	const std::vector<Object>& Objects() const;
	void UpdateForce();
	void UpdateSpeed();
	void UpdatePos();
	void Clean();
	void Update();

private:
	std::vector<Object> _objects;
	std::vector<Object> _farObjects;
};
