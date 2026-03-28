// ◦ Xyz ◦

#include "Generetors.h"
#include <Files/Settings.h>
#include <Common/JsonHelper.h>
#include <Common/Common.h>

using namespace GeneratorSpace;

std::vector<Body> OnOrbit::Generate()
{
	auto& jsonData = GetData();
	jsonData["generator"] = Engine::GetClassName<OnOrbit>();

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
	int i = 1;

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

	float sumMass = 0.f;
	glm::vec3 centerMass(0.f, 0.f, 0.f);

	for (size_t i = 0; i < bodies.size() - 1; ++i) {
		auto& body = bodies[i];
		centerMass += body.pos * body.mass;
		sumMass += body.mass;
	}

	centerMass /= sumMass;

	auto& mainBody = bodies.emplace_back();
	mainBody.mass = sumMass * mainMassFactor;
	mainBody.pos = centerMass;

	for (size_t i = 0; i < bodies.size() - 1; ++i) {
		auto& body = bodies[i];
		body.velocity = GetVelocityOnOrbit(body, mainBody);
	}

	return bodies;
}
