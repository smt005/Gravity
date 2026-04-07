// ◦ Xyz ◦

#include "SpaceManager.h"
#include <numbers>
#include <chrono>
#include <glm/glm.hpp>
#include <Files/FileManager.h>
#include <Common/Common.h>
#include <Logs.h>
#include "Generators/Generetors.h"
#include "../DebugContext.h"
#include "../Windows/InfoWindow.h"
#include "Space.h"
#include "DefaultSpace.h"
#include "MainThreadSpace.h"
#include "MainThreadAllInBodySpace.h"
//#include "XXX.h"

namespace {
	std::string_view spaceDirectory = "Spaces";
	std::string_view curentSpaceFileNamePath = "Spaces/Current.space";
}

Space& SpaceManager::Current()
{
	return *CurrentPtr();
}

const Space::Ptr& SpaceManager::CurrentPtr()
{
	if (!currentSpace) {
		return GetPtr<Spaces::Default>(true);
	}
	return currentSpace;
}

void SpaceManager::Update(double deltaTime)
{
	DebugContext::Instance().Clean();
	CheckOverload(deltaTime);

	for (size_t it = 1; it <= countOfIteration; ++it) {
		Current().Update();
	}
	
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

	if (MakeSpace<Spaces::Default>(className)) {}
	else if (MakeSpace<MainThread>(className)) {}
	else if (MakeSpace<MainThreadAllInBody>(className)) {}
	//else if (MakeSpace<XXX>(className)) {}
	else {
		SetCurrentPtr<Spaces::Default>();
	}

	if (!LoadSpace()) {
		GeneratorSpace::Load();
	}
}

void SpaceManager::Save()
{
	auto& settingData = GeneratorSpace::GetData();
	settingData["class"] = Current().GetName();
	SaveSpace();
}

bool SpaceManager::LoadSpace()
{
	const std::vector<BodyData> spaceData = Engine::FileManager::Get("write").ReadFile<std::vector<BodyData>>(curentSpaceFileNamePath);
	if (spaceData.empty()) {
		return false;
	}

	auto& space = Current();
	space.Clear();
	space.AddBodies(spaceData);

	return true;
}

void SpaceManager::SaveSpace(bool addTimeToName)
{
	std::vector<BodyData> spaceData = Current().GetBodies();

	if (addTimeToName) {
		using namespace std::chrono;
		auto now = system_clock::now();
		auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
		const std::string timeStr = std::format("{:%d_%m_%Y_%H_%M_%S}", now);
		const std::string fileNamePath = TO_STRING("Spaces/Current_{}.space", timeStr);
		Engine::FileManager::Get("write").WriteFile(spaceData, fileNamePath);
	}
	else {
		Engine::FileManager::Get("write").WriteFile(spaceData, curentSpaceFileNamePath);
	}
}

Json& SpaceManager::GetSettingData(std::string_view path, bool create)
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

	static Json defaultJsonData;
	return defaultJsonData;
}

void SpaceManager::StopUpdate()
{
	countOfIteration.store(0.f);
}

void SpaceManager::CheckOverload(double deltaTime)
{
	// TODO:
	if (!Engine::IsDebugging() && deltaTime > 0.25) {
		if (!Engine::IsDebugging() && Current().type == Space::ThreadType::IN_MAIN) {
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

	/*for (auto& body : bodies) {
		debugContext.minForce = 0.f;// std::min(debugContext.minForce, glm::length(body.force));
		debugContext.minMass = std::min(debugContext.minMass, glm::length(body.mass));
		debugContext.minVelocity = std::min(debugContext.minVelocity, glm::length(body.velocity));
		debugContext.maxForce = 0.f;// std::max(debugContext.maxForce, glm::length(body.force));
		debugContext.maxMass = std::max(debugContext.maxMass, glm::length(body.mass));
		debugContext.maxVelocity = std::max(debugContext.maxVelocity, glm::length(body.velocity));
		debugContext.sumMass += body.mass;
	}*/
}

glm::vec3 SpaceManager::CenteMassSpace()
{
	// TODO
	return {};
}
