// ◦ Xyz ◦

#include "Generetors.h"
#include <Files/Settings.h>
#include <Common/JsonHelper.h>
#include <Common/Common.h>

using namespace GeneratorSpace;

std::vector<Body> Default::Generate()
{
	GetData()["generator"] = Engine::GetClassName<Default>();
	std::vector<Body> bodies(3);

	auto& mainBody = bodies[0];
	mainBody.mass = 1000;

	{
		auto& body = bodies[1];
		body.mass = 10;
		body.pos.x = 100.f;
		body.velocity = GetVelocityOnOrbit(body, mainBody);
	}
	{
		auto& body = bodies[2];
		body.mass = 10;
		body.pos.x = -100.f;
		body.velocity = GetVelocityOnOrbit(body, mainBody);
	}

	return bodies;
}
