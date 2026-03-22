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

	virtual ~Space() = default;
	Space& operator = (const Space& in);
	Space& operator = (const std::vector<Object>& objects);

	virtual void Generate(size_t count, size_t radius, int typeGenerate);
	virtual void Clean();
	virtual void Update();
	
	void SwitchPause();
	const std::string& GetName() const;
	size_t GetId() const;
	bool IsPaused() const;
	
	const std::vector<Object>& Objects() const;
	glm::vec3 PosOfMinSpeedObject() const;
	glm::vec3 PosOfMaxSpeedObject() const;
	glm::vec3 PosOfMinMassObject() const;
	glm::vec3 PosOfMaxMassObject() const;

public:
	inline static int deltaTime = 1;
	inline static int countOfIteration = 1;

	// TODO:
public:
	int typeTemp = 0;
	int countTemp = 0;
	int sizeTemp = 0;

protected:
	bool _pause = false;
	mutable std::string _name;
	mutable size_t _id;
	std::vector<Object> _objects;
};
