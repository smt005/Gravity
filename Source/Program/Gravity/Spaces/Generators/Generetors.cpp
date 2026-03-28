// ◦ Xyz ◦

#include "Generetors.h"
#include <glm/geometric.hpp>
#include <Files/Settings.h>
#include <Common/JsonHelper.h>
#include <Common/Common.h>
#include "../SpaceManager.h"

template <typename TGenerator>
void ApplyGenerate()
{
	auto& space = SpaceManager::Current();
	space.Clear();
	space.AddBodies(TGenerator::Generate());
}

template <typename TGenerator>
bool ChooseGenerate(const std::string& className)
{
	if (className == Engine::GetClassName<TGenerator>()) {
		ApplyGenerate<TGenerator>();
		return true;
	}
	return false;
}

std::string_view GeneratorSpace::NameData()
{
	static std::string_view nameData = "GeneratorSpace";
	return nameData;
}

nlohmann::json& GeneratorSpace::GetData()
{
	auto* jsonData = Engine::Settings::Instance().JsonData(NameData(), true);
	if (!jsonData) {
		throw std::logic_error("GeneratorSpace::Save fail!");
	}

	return *jsonData;
}

void GeneratorSpace::Load()
{
	const std::string className = Engine::GetJsonValue<std::string>("generator", GetData());

	if (ChooseGenerate<Box>(className)) {
		return;
	}
	if (ChooseGenerate<Circle>(className)) {
		return;
	}
	if (ChooseGenerate<Sphere>(className)) {
		return;
	}
	if (ChooseGenerate<OnOrbit>(className)) {
		return;
	}
	if (ChooseGenerate<OnPlaneOrbit>(className)) {
		return;
	}

	ApplyGenerate<Default>();
}

glm::vec3 GeneratorSpace::GetVelocityOnOrbit(Body& body, const Body& mainBody)
{
	glm::vec3 gravityVector = body.pos - mainBody.pos;
	glm::vec3 normalizeGravityVector = glm::normalize(gravityVector);

	float g90 = std::numbers::pi / 2.f;
	glm::vec3 velocity(normalizeGravityVector.x * std::cos(g90) - normalizeGravityVector.y * std::sin(g90),
		normalizeGravityVector.x * std::sin(g90) + normalizeGravityVector.y * std::cos(g90),
		0.f);

	velocity *= std::sqrtf(Body::constantGravity * mainBody.mass / glm::length(gravityVector));
	return velocity;
}
