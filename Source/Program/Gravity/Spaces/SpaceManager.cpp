// ◦ Xyz ◦

#include "SpaceManager.h"
#include <numbers>
#include <glm/glm.hpp>
#include <Common/Common.h>
#include "Space.h"
#include "DefaultSpace.h"
#include "MainThreadSpace.h"
#include "../DebugContext.h"
#include "../Windows/InfoWindow.h"
#include <Logs.h>

Space& SpaceManager::Current()
{
	return *CurrentPtr();
}

const Space::Ptr& SpaceManager::CurrentPtr()
{
	if (!currentSpace) {
		return GetPtr<DefaultSpace>(true);
	}
	return currentSpace;
}

void SpaceManager::Update(double deltaTime)
{
	DebugContext::Instance().Clean();
	CheckOverload(deltaTime);
	Current().Update();
	CollectDebugData();
}

void SpaceManager::Load()
{
	Space::Ptr currentPtr;
	const std::string className = Engine::GetClassName<SpaceManager>();
	auto& settings = Engine::Settings::Instance();
	auto currentSpaceName = settings[TO_STRING("{}/class", className)];

	if (currentSpaceName == Engine::GetClassName<DefaultSpace>()) {
		currentPtr = SetCurrentPtr<DefaultSpace>();
	}
	else if (currentSpaceName == Engine::GetClassName<MainThreadSpace>()) {
		currentPtr = SetCurrentPtr<MainThreadSpace>();
	}
	else {
		currentPtr = SetCurrentPtr<DefaultSpace>();
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

void SpaceManager::CollectDebugData()
{
	DebugContext& debugContext = DebugContext::Instance();

	for (auto& body : Current().Bodies()) {
		debugContext.minForce = std::min(debugContext.minForce, glm::length(body.force));
		debugContext.minMass = std::min(debugContext.minMass, glm::length(body.mass));
		debugContext.minVelocity = std::min(debugContext.minVelocity, glm::length(body.velocity));
		debugContext.maxForce = std::max(debugContext.maxForce, glm::length(body.force));
		debugContext.maxMass = std::max(debugContext.maxMass, glm::length(body.mass));
		debugContext.maxVelocity = std::max(debugContext.maxVelocity, glm::length(body.velocity));
		debugContext.sumMass += body.mass;
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
	
	std::vector<Body> bodies;
	bodies.reserve(count);

	glm::vec3 vec3(1.f, 0.f, 0.f);

	for (int i = 0; i < count; ++i) {
		auto& object = bodies.emplace_back();

		object.pos.x = Engine::Random(-(float)spaceRange, (float)spaceRange);
		object.pos.y = Engine::Random(-(float)spaceRange, (float)spaceRange);
		object.pos.z = Engine::Random(-(float)spaceRange, (float)spaceRange);

		float velocity = 0.0;// 05f;
		object.velocity.x = Engine::Random(-(float)velocity, (float)velocity);
		object.velocity.y = Engine::Random(-(float)velocity, (float)velocity);
		object.velocity.z = Engine::Random(-(float)velocity, (float)velocity);

		object.mass = Engine::Random(0.1f, 1.f);
	}

	Current().Clear();
	Current().AddBodies(std::move(bodies));
}

void SpaceManager::GeneratePlaneSphere(int count, float minSpaceRange, float spaceRange)
{
	auto& generationData = GetSettingData("generationData", true);
	generationData["type"] = 1;
	generationData["count"] = (int)count;
	generationData["minSize"] = (int)minSpaceRange;
	generationData["size"] = (int)spaceRange;

	std::vector<Body> bodies;
	bodies.reserve(count);

	glm::vec3 vec3(1.f, 0.f, 0.f);

	for (int i = 0; i < count; ++i) {
		auto& object = bodies.emplace_back();

		float angle = Engine::Random<double>(0, std::numbers::pi * 2);

		glm::vec3 objectVec3(0.f, 0.f, 0.f);
		objectVec3.x = vec3.x * cos(angle) - vec3.y * sin(angle);
		objectVec3.y = vec3.x * sin(angle) + vec3.y * cos(angle);
		objectVec3 *= Engine::Random(minSpaceRange, (float)spaceRange);

		object.pos = objectVec3;
		object.mass = Engine::Random(0.1f, 1.f);
	}

	Current().Clear();
	Current().AddBodies(std::move(bodies));
}

void SpaceManager::GenerateOnOrbitSphere(int count, float minSpaceRange, float spaceRange)
{
	auto& generationData = GetSettingData("generationData", true);
	float massFactor = Engine::GetJsonValue("mainBodyMassFactor", generationData, 1.f);
	generationData["type"] = 2;
	generationData["count"] = (int)count;
	generationData["minSize"] = (int)minSpaceRange;
	generationData["size"] = (int)spaceRange;

	std::vector<Body> bodies;
	bodies.reserve(count);

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

		auto& body = bodies.emplace_back();
		body.mass = mass;
		body.pos = pos;
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
	mainBody.mass = sumMass * massFactor;
	mainBody.pos = centerMass;

	for (size_t i = 0; i < bodies.size() - 1; ++i) {
		auto& body = bodies[i];
		body.velocity = GetVelocityOnOrbit(body, mainBody);
	}

	Current().Clear();
	Current().AddBodies(std::move(bodies));
}

glm::vec3 SpaceManager::GetVelocityOnOrbit(Body& body, const Body& mainBody)
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

glm::vec3 SpaceManager::PosOfMinSpeedObject()
{
	float velocity = std::numeric_limits<float>::max();
	const Body* obj = nullptr;

	for (const auto& body : Current().Bodies()) {
		if (glm::length(body.velocity) < velocity) {
			velocity = glm::length(body.velocity);
			obj = &body;
		}
	}

	if (!obj) {
		return {};
	}
	
	PRINT("MIN VELOCITY: {}, {} mass: {} pos: {}", velocity, obj->velocity, obj->mass, obj->pos);
	return obj->velocity;
}

glm::vec3 SpaceManager::PosOfMaxSpeedObject()
{
	float velocity = 0.f;
	const Body* obj = nullptr;

	for (const auto& body : Current().Bodies()) {
		if (glm::length(body.velocity) > velocity) {
			velocity = glm::length(body.velocity);
			obj = &body;
		}
	}
	
	if (!obj) {
		return {};
	}

	PRINT("MAX VELOCITY: {}, {} mass: {} pos: {}", velocity, obj->velocity, obj->mass, obj->pos);
	return obj->velocity;
}

glm::vec3 SpaceManager::PosOfMinMassObject()
{
	glm::vec3 pos(0.f, 0.f, 0.f);
	float mass = std::numeric_limits<float>::max();
	const Body* obj = nullptr;

	for (const auto& body : Current().Bodies()) {
		if (body.mass < mass) {
			mass = body.mass;
			obj = &body;
		}
	}
	
	if (!obj) {
		return {};
	}

	PRINT("MIN MASS: {} velocity: {} pos: {}", mass, glm::length(obj->velocity), obj->pos);
	return obj->pos;
}

glm::vec3 SpaceManager::PosOfMaxMassObject()
{
	const Body* obj = nullptr;
	float mass = 0.f;

	for (const auto& body : Current().Bodies()) {
		if (body.mass > mass) {
			mass = body.mass;
			obj = &body;
		}
	}

	if (!obj) {
		return {};
	}

	PRINT("MAX MASS: {} velocity: {} pos: {} arrVec3: {}", mass, glm::length(obj->velocity), obj->pos);
	return obj->pos;
}
