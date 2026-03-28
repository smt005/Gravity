// ◦ Xyz ◦

#include "SpaceManager.h"
#include <numbers>
#include <glm/glm.hpp>
#include <Common/Common.h>
#include "Space.h"
#include "DefaultSpace.h"
#include "MainThreadSpace.h"
#include "MainThreadProtSpace.h"
#include "ParallelThreadSpace.h"
#include "Generators/Generetors.h"
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
	Current().Bodies(bodies);
	CollectDebugData();
}

template <typename TSpace>
bool MakeSpace(const std::string& name)
{
	if (name == Engine::GetClassName<TSpace>()) {
		SpaceManager::SetCurrentPtr<TSpace>();
		return true;
	}
	return false;
}

void SpaceManager::Load()
{
	
	const std::string className = Engine::GetJsonValue<std::string>("class", GeneratorSpace::GetData());

	if (MakeSpace<DefaultSpace>(className)) {}
	else if (MakeSpace<MainThreadProtSpace>(className)) {}
	else if (MakeSpace<MainThreadSpace>(className)) {}
	else if (MakeSpace<MainThreadSpace>(className)) {}
	else if (MakeSpace<ParallelThreadSpace>(className)) {}
	else {
		SetCurrentPtr<DefaultSpace>();
	}

	GeneratorSpace::Load();
}

void SpaceManager::Save()
{
	auto& settingData = GeneratorSpace::GetData();
	settingData["class"] = Current().GetName();
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
	countOfIteration.store(0.f);
}

void SpaceManager::GetBodyPositions(std::vector<float>& positions)
{
	std::vector<float> tempPositions;
	tempPositions.reserve(bodies.size() * 3);

	for (const auto& body : bodies) {
		tempPositions.emplace_back(body.pos.x);
		tempPositions.emplace_back(body.pos.y);
		tempPositions.emplace_back(body.pos.z);
	}

	std::swap(positions, tempPositions);
}

void SpaceManager::GetBodyPositions(std::vector<glm::vec3>& positions)
{
	std::vector<glm::vec3> tempPositions;
	tempPositions.reserve(bodies.size());

	for (const auto& body : bodies) {
		tempPositions.emplace_back(body.pos);
	}

	std::swap(positions, tempPositions);
}


void SpaceManager::CheckOverload(double deltaTime)
{
	// TODO:
	if (!Engine::IsDebugging() && deltaTime > 0.25) {
		if (!Engine::IsDebugging()) {
			SpaceManager::StopUpdate();

			using namespace Windows;
			std::list<InfoWindow::Element> elements;
			elements.emplace_back(InfoWindow::TypeElement::TEXT, TO_STRING("\nSTOP: LOW FPS\ndeltaTime: {}\n", deltaTime), nullptr);
			elements.emplace_back(InfoWindow::TypeElement::CLOSE_BUTTON, "Run.", []() { SpaceManager::countOfIteration.store(1.f); });
			elements.emplace_back(InfoWindow::TypeElement::CLOSE_BUTTON, "", nullptr);
			InfoWindow::ShowMessageWindow(std::move(elements));
		}
		else {
			PRINT("\nSTOP: LOW FPS\ndeltaTime: {}\n", deltaTime);
		}
	}
}

void SpaceManager::CollectDebugData()
{
	DebugContext& debugContext = DebugContext::Instance();

	for (auto& body : bodies) {
		debugContext.minForce = std::min(debugContext.minForce, glm::length(body.force));
		debugContext.minMass = std::min(debugContext.minMass, glm::length(body.mass));
		debugContext.minVelocity = std::min(debugContext.minVelocity, glm::length(body.velocity));
		debugContext.maxForce = std::max(debugContext.maxForce, glm::length(body.force));
		debugContext.maxMass = std::max(debugContext.maxMass, glm::length(body.mass));
		debugContext.maxVelocity = std::max(debugContext.maxVelocity, glm::length(body.velocity));
		debugContext.sumMass += body.mass;
	}

	auto& space = Current();
	debugContext.subProgress = space.GetSubProgress();
	debugContext.progress = space.GetProgress();
}

glm::vec3 SpaceManager::CenteMassSpace()
{
	// TODO
	return {};
}

glm::vec3 SpaceManager::PosOfMinSpeedObject()
{
	float velocity = std::numeric_limits<float>::max();
	const Body* obj = nullptr;

	for (const auto& body : bodies) {
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

	for (const auto& body : bodies) {
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

	for (const auto& body : bodies) {
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

	for (const auto& body : bodies) {
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
