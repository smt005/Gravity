// ◦ Xyz ◦
#pragma once

#include <vector>
#include <string>
#include <glm/vec3.hpp>
#include <mystd_memory.h>
#include <Common/Common.h>
#include "BodyData.h"

class Space
{
public:
	using Ptr = mystd::shared_ptr<Space>;

	Space() = default;
	virtual ~Space() = default;
	
	Space(const Space&) = delete;
	Space(Space&&) = delete;
	void operator = (const Space& in) = delete;
	void operator = (Space&& in) = delete;

	virtual void Clear() = 0;
	virtual void Update() = 0;
	virtual void AddBody(const BodyData& body) = 0;
	virtual void AddBodies(const std::vector<BodyData>& bodies) = 0;
	virtual void Bodies(std::vector<BodyData>& bodies) = 0;
	virtual std::vector<BodyData> GetBodies() = 0;
	virtual float GetSubProgress() const {
		return -1;
	}
	virtual float GetProgress() const {
		return -1;
	}

	const std::string& GetName() const {
		if (_name.empty()) _name = Engine::ExtractClassName(typeid(*this).name());
		return _name;
	}
	size_t GetId() const {
		if (!_id) _id = typeid(*this).hash_code();
		return _id;
	}

private:
	mutable std::string _name;
	mutable size_t _id;

public:
	inline static float constantGravity = 6.67430e-11f; // Гравитационная постоянная
};
