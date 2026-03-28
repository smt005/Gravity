// ◦ Xyz ◦
#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "../Body.h"

namespace GeneratorSpace
{
	struct Default
	{
		static std::vector<Body> Generate();
	};

	struct Box
	{
		static std::vector<Body> Generate();
	};

	struct Circle
	{
		static std::vector<Body> Generate();
	};

	struct Sphere
	{
		static std::vector<Body> Generate();
	};

	struct OnOrbit
	{
		static std::vector<Body> Generate();
	};

	struct OnPlaneOrbit
	{
		static std::vector<Body> Generate();
	};
	
	std::string_view NameData();
	nlohmann::json& GetData();
	void Load();

	glm::vec3 GetVelocityOnOrbit(Body& body, const Body& mainBody);
}
