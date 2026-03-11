// ◦ Xyz ◦
#pragma once

#include "Singletone.h"
#include "Object.h"
#include <vector>
#include <glm/vec3.hpp>

class Space final : public mystd::Singletone<Space> {
public:
	virtual void Generate(size_t count, size_t radius);
	virtual void UpdateForce();
	virtual void UpdateSpeed();
	virtual void UpdatePos();
	virtual void Clean();
	virtual void Update();
	
	void SwitchPause();
	bool IsPaused() const;

	const std::vector<Object>& Objects() const;
	glm::vec3 PosOfMinSpeedObject() const;
	glm::vec3 PosOfMaxSpeedObject() const;
	glm::vec3 PosOfMinMassObject() const;
	glm::vec3 PosOfMaxMassObject() const;

private:
	bool _pause = true;
	std::vector<Object> _objects;
};
