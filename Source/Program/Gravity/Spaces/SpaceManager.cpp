// ◦ Xyz ◦

#include "SpaceManager.h"
#include <numbers>
#include <glm/glm.hpp>
#include <Common/Common.h>
#include "Space.h"
#include "OneThreadSpace.h"
#include "../Windows/InfoWindow.h"

Space& SpaceManager::Current()
{
	return *CurrentPtr();
}

const Space::Ptr& SpaceManager::CurrentPtr()
{
	if (!currentSpace) {
		return GetPtr<Space>(true);
	}
	return currentSpace;
}

void SpaceManager::Update(double deltaTime)
{
	CheckOverload(deltaTime);
	Current().Update();
}

void SpaceManager::Load()
{
	Space::Ptr currentPtr;
	const std::string className = Engine::GetClassName<SpaceManager>();
	auto& settings = Engine::Settings::Instance();
	auto currentSpaceName = settings[TO_STRING("{}/class", className)];

	if (currentSpaceName == Engine::GetClassName<Space>()) {
		currentPtr = SetCurrentPtr<Space>();
	}
	else if (currentSpaceName == Engine::GetClassName<OneThreadSpace>()) {
		currentPtr = SetCurrentPtr<OneThreadSpace>();
	}
	else {
		currentPtr = SetCurrentPtr<Space>();
	}

	auto generationData = settings[TO_STRING("{}/generationData", className)];
	int type = Engine::GetJsonValue<int>("type", generationData, 0);
	int count = Engine::GetJsonValue<int>("count", generationData, 200);
	int minSize = Engine::GetJsonValue<int>("minSize", generationData, 1);
	int size = Engine::GetJsonValue<int>("size", generationData, 200);

	GenerateType gType = GenerateType::NONE;
	switch (type) {
	case 0: gType = GenerateType::BOX; break;
	case 1: gType = GenerateType::PLANE_SPHERE; break;
	case 2: gType = GenerateType::ORBIT_ON_MAIN_BODT; break;
	default: gType = GenerateType::NONE;
	}
	
	Generate(gType, count, minSize, size);
}

void SpaceManager::Save()
{
	auto& generationData = GetSettingData("", true);
	generationData["class"] = Current().GetName();
}

nlohmann::json& SpaceManager::GetSettingData(std::string_view path, bool create)
{
	const std::string className = Engine::GetClassName<SpaceManager>();
	auto& settings = Engine::Settings::Instance();

	if (auto* spaceManager = settings.JsonData(className, true)) {
		if (path.empty()) {
			return *spaceManager;
		}
		else {
			return (*spaceManager)[path];
		}
	}

	static nlohmann::json defaultJsonData;
	return defaultJsonData;
}

void SpaceManager::StopUpdate()
{
	countOfIteration = 0;
}

void SpaceManager::CheckOverload(double deltaTime)
{
	// TODO:
	if (deltaTime > 0.25) {
		SpaceManager::StopUpdate();

		using namespace Windows;
		std::list<InfoWindow::Element> elements;
		elements.emplace_back(InfoWindow::TypeElement::TEXT, TO_STRING("\nSTOP: LOW FPS\ndeltaTime: {}\n", deltaTime), nullptr);
		elements.emplace_back(InfoWindow::TypeElement::CLOSE_BUTTON, "Run.", []() {SpaceManager::countOfIteration = 1; });
		elements.emplace_back(InfoWindow::TypeElement::CLOSE_BUTTON, "", nullptr);
		InfoWindow::ShowMessageWindow(std::move(elements));
	}
}


glm::vec3 SpaceManager::CenteMassSpace()
{
	// TODO
	return {};
}

void SpaceManager::GenerateBox(int count, float minSpaceRange, float spaceRange)
{
	auto& generationData = GetSettingData("generationData", true);
	generationData["type"] = 0;
	generationData["count"] = (int)count;
	generationData["minSize"] = (int)minSpaceRange;
	generationData["size"] = (int)spaceRange;

	Space& space = Current();
	auto& objects = space._objects;
	objects.clear();
	objects.reserve(count);

	glm::vec3 vec3(1.f, 0.f, 0.f);

	for (int i = 0; i < count; ++i) {
		auto& object = objects.emplace_back();

		object.pos.x = Engine::Random(-(float)spaceRange, (float)spaceRange);
		object.pos.y = Engine::Random(-(float)spaceRange, (float)spaceRange);
		object.pos.z = Engine::Random(-(float)spaceRange, (float)spaceRange);

		float velocity = 0.0;// 05f;
		object.velocity.x = Engine::Random(-(float)velocity, (float)velocity);
		object.velocity.y = Engine::Random(-(float)velocity, (float)velocity);
		object.velocity.z = Engine::Random(-(float)velocity, (float)velocity);

		object.mass = Engine::Random(0.1f, 1.f);
	}
}

void SpaceManager::GeneratePlaneSphere(int count, float minSpaceRange, float spaceRange)
{
	auto& generationData = GetSettingData("generationData", true);
	generationData["type"] = 1;
	generationData["count"] = (int)count;
	generationData["minSize"] = (int)minSpaceRange;
	generationData["size"] = (int)spaceRange;

	Space& space = Current();
	auto& objects = space._objects;
	objects.clear();
	objects.reserve(count);

	glm::vec3 vec3(1.f, 0.f, 0.f);

	for (int i = 0; i < count; ++i) {
		auto& object = objects.emplace_back();

		float angle = Engine::Random<double>(0, std::numbers::pi * 2);

		glm::vec3 objectVec3(0.f, 0.f, 0.f);
		objectVec3.x = vec3.x * cos(angle) - vec3.y * sin(angle);
		objectVec3.y = vec3.x * sin(angle) + vec3.y * cos(angle);
		objectVec3 *= Engine::Random(minSpaceRange, (float)spaceRange);

		object.pos = objectVec3;
		object.mass = Engine::Random(0.1f, 1.f);
	}
}

void SpaceManager::GenerateOnOrbitSphere(int count, float minSpaceRange, float spaceRange)
{
	auto& generationData = GetSettingData("generationData", true);
	float massFactor = Engine::GetJsonValue("mainBodyMassFactor", generationData, 1.f);
	generationData["type"] = 2;
	generationData["count"] = (int)count;
	generationData["minSize"] = (int)minSpaceRange;
	generationData["size"] = (int)spaceRange;

    Space& space = Current();
	auto& objects = space._objects;
	objects.clear();
	objects.reserve(count);

	float mainRadius = 10.f;
	glm::vec3 mainPos(0.f, 0.f, 0.f);
	minSpaceRange = minSpaceRange < mainRadius ? mainRadius : minSpaceRange;

	int i = 1;
	while (i < count) {
		glm::vec3 pos;
		pos.x = Engine::Random(-spaceRange, spaceRange);
        pos.y = Engine::Random(-spaceRange, spaceRange);
        pos.z = 0.f;

        float radius = glm::length(pos - mainPos);

        if (radius > spaceRange) {
            continue;
        }

        if (radius < minSpaceRange) {
            continue;
        }

		float minMass = 1.f;
		float maxMass = 5.f;
		float mass = Engine::Random(minMass, maxMass);

		++i;

		auto& body = Current()._objects.emplace_back();
		body.mass = mass;
		body.pos = pos;
    }

	float sumMass = 0.f;
	glm::vec3 centerMass(0.f, 0.f, 0.f);

	for (size_t i = 0; i < Current()._objects.size() - 1; ++i) {
		auto& body = Current()._objects[i];
		centerMass += body.pos * body.mass;
		sumMass += body.mass;
	}

	centerMass /= sumMass;

	auto& mainBody = objects.emplace_back();
	mainBody.mass = sumMass * massFactor;
	mainBody.pos = centerMass;

	for (size_t i = 0; i < Current()._objects.size() - 1; ++i) {
		auto& body = Current()._objects[i];
		body.velocity = GetVelocityOnOrbit(body, mainBody);
	}
}

void SpaceManager::AddBodyOnOrbit(const glm::vec3& pos, const Object& mainBody)
{
	Space& space = Current();
	auto& objects = space._objects;

	float minMass = 1.f;
	float maxMass = 5.f;
	float mass = Engine::Random(minMass, maxMass);

	glm::vec3 gravityVector = pos - mainBody.pos;
	glm::vec3 normalizeGravityVector = glm::normalize(gravityVector);

	float g90 = std::numbers::pi / 2.f;
	glm::vec3 velocity(normalizeGravityVector.x * std::cos(g90) - normalizeGravityVector.y * std::sin(g90),
		normalizeGravityVector.x * std::sin(g90) + normalizeGravityVector.y * std::cos(g90),
		0.f);
	velocity *= std::sqrtf(Object::gForce * mainBody.mass / glm::length(gravityVector));

	auto& body = Current()._objects.emplace_back();
	body.mass = mass;
	body.pos = pos;
	body.velocity = velocity;
}

glm::vec3 SpaceManager::GetVelocityOnOrbit(Object& body, const Object& mainBody)
{
	glm::vec3 gravityVector = body.pos - mainBody.pos;
	glm::vec3 normalizeGravityVector = glm::normalize(gravityVector);

	float g90 = std::numbers::pi / 2.f;
	glm::vec3 velocity(normalizeGravityVector.x * std::cos(g90) - normalizeGravityVector.y * std::sin(g90),
		normalizeGravityVector.x * std::sin(g90) + normalizeGravityVector.y * std::cos(g90),
		0.f);
	velocity *= std::sqrtf(Object::gForce * mainBody.mass / glm::length(gravityVector));
	return velocity;
}
