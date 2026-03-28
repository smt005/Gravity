// ◦ Xyz ◦

#include "Generetors.h"
#include <Files/Settings.h>
#include <Common/JsonHelper.h>
#include <Common/Common.h>

using namespace GeneratorSpace;

std::vector<Body> Sphere::Generate()
{
	auto& jsonData = GetData();
	jsonData["generator"] = Engine::GetClassName<Box>();

	const int count = Engine::GetJsonValue("count", jsonData, 300);
	const int size = Engine::GetJsonValue("size", jsonData, 100);
	const float minMass = Engine::GetJsonValue("minMass", jsonData, 0.1f);
	const float maxMass = Engine::GetJsonValue("maxMass", jsonData, 1.0f);
	const float mainMassFactor = Engine::GetJsonValue("mainMassFactor", jsonData, 1.0f);

	const float minPos = -static_cast<float>(size) / 2.f;
	const float maxPos =  static_cast<float>(size) / 2.f;
	
	std::vector<Body> bodies;
	bodies.reserve(count);

	glm::vec3 pos(0.f, 0.f, 0.f);
	int i = 0;

	while (i < count) {	
		pos.x = Engine::Random(minPos, maxPos);
		pos.y = Engine::Random(minPos, maxPos);
		pos.z = Engine::Random(minPos, maxPos);

		const float radius = glm::length(pos);
		if (radius > maxPos) {
			continue;
		}

		auto& body = bodies.emplace_back();
		body.mass = Engine::Random(minMass, maxMass);;
		body.pos = pos;

		++i;
	}
	
	return bodies;
}
