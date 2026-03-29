// ◦ Xyz ◦

#include "Generetors.h"
#include <Files/Settings.h>
#include <Common/JsonHelper.h>
#include <Common/Common.h>

using namespace GeneratorSpace;

std::vector<BodyData> Box::Generate()
{
	auto& jsonData = GetData();
	jsonData["generator"] = Engine::GetClassName<Box>();

	const int count = Engine::GetJsonValue("count", jsonData, 100);
	const int size = Engine::GetJsonValue("size", jsonData, 100);
	const float minMass = Engine::GetJsonValue("minMass", jsonData, 0.1f);
	const float maxMass = Engine::GetJsonValue("maxMass", jsonData, 1.0f);

	const float minPos = -static_cast<float>(size) / 2.f;
	const float maxPos =  static_cast<float>(size) / 2.f;

	std::vector<BodyData> bodies;
	bodies.reserve(count);

	for (size_t i = 0; i < count; ++i) {
		auto& bodie = bodies.emplace_back();

		bodie.mass = Engine::Random(minMass, maxMass);
		bodie.pos.x = Engine::Random(minPos, maxPos);
		bodie.pos.y = Engine::Random(minPos, maxPos);
		bodie.pos.z = Engine::Random(minPos, maxPos);
	}

	return bodies;
}
