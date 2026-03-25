// ◦ Xyz ◦
#pragma once

#include "Object.h"
#include <vector>
#include <string>
#include <glm/vec3.hpp>
#include <mystd_memory.h>

using Body = Object;

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
	virtual void CollectDebugData() const;

	void SwitchPause();
	const std::string& GetName() const;
	size_t GetId() const;
	bool IsPaused() const;

	const std::vector<Object>& Objects() const;
	const std::vector<Body>& Bodies() const;
	glm::vec3 PosOfMinSpeedObject() const;
	glm::vec3 PosOfMaxSpeedObject() const;
	glm::vec3 PosOfMinMassObject() const;
	glm::vec3 PosOfMaxMassObject() const;

public:
	inline static float progress = -1.f;
	inline static float subProgress = -1.f;

	// TODO:
public:
	int typeTemp = 0;
	int countTemp = 0;
	int sizeTemp = 0;

protected:
public: // TODO
	bool _pause = false;
	mutable std::string _name;
	mutable size_t _id;
	std::vector<Body> _objects;
};
