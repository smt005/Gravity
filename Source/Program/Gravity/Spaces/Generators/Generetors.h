// ◦ Xyz ◦
#pragma once

#include <string>
#include <vector>
#include <Common/JsonHelper.h>
#include "../BodyData.h"

namespace GeneratorSpace
{
	struct Default
	{
		static std::vector<BodyData> Generate();
	};

	struct Box
	{
		static std::vector<BodyData> Generate();
	};

	struct Circle
	{
		static std::vector<BodyData> Generate();
	};

	struct Sphere
	{
		static std::vector<BodyData> Generate();
	};

	struct OnOrbit
	{
		static std::vector<BodyData> Generate();
	};

	struct OnPlaneOrbit
	{
		static std::vector<BodyData> Generate();
	};
	
	std::string_view NameData();
	Json& GetData();
	void Load();

	glm::vec3 GetVelocityOnOrbit(BodyData& body, const BodyData& mainBody);
}
