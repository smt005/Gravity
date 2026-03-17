// ◦ Xyz ◦
#pragma once

#include "Object.h"
#include <vector>
#include <string>
#include <glm/vec3.hpp>
#include <mystd_memory.h>

class Space
{
public:
	using Ptr = mystd::shared_ptr<Space>;

	Space();

	virtual void Generate(size_t count, size_t radius);
	virtual void Clean();
	virtual void Update();
	
	void SwitchPause();
	const std::string& GetName() const;
	bool IsPaused() const;

	const std::vector<Object>& Objects() const;
	glm::vec3 PosOfMinSpeedObject() const;
	glm::vec3 PosOfMaxSpeedObject() const;
	glm::vec3 PosOfMinMassObject() const;
	glm::vec3 PosOfMaxMassObject() const;

protected:
	void SetName(std::string name);

protected:
	bool _pause = true;
	std::string _name;
	std::vector<Object> _objects;
};
