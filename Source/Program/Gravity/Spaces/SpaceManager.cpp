// ◦ Xyz ◦

#include "SpaceManager.h"
#include "Space.h"
#include "OneThreadSpace.h"

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

void SpaceManager::Update()
{
	Current().Update();
}

void SpaceManager::Load()
{
	Space::Ptr currentPtr;
	const std::string className = Engine::GetClassName<SpaceManager>();
	auto& settings = Engine::Settings::Instance();
	auto currentSpaceName = settings[TO_STRING("{}/currentSpace", className)];

	if (currentSpaceName == Engine::GetClassName<Space>()) {
		currentPtr = SetCurrentPtr<Space>();
	}
	else if (currentSpaceName == Engine::GetClassName<OneThreadSpace>()) {
		currentPtr = SetCurrentPtr<OneThreadSpace>();
	}
	else {
		currentPtr = SetCurrentPtr<Space>();
	}

	auto generateData = settings[TO_STRING("{}/generateData", className)];
	int type = Engine::GetJsonValue<int>("type", generateData, 0);
	int count = Engine::GetJsonValue<int>("count", generateData, 200);
	int size = Engine::GetJsonValue<int>("size", generateData, 200);

	currentPtr->Generate(count, size, type);
}

void SpaceManager::Save()
{
	const std::string className = Engine::GetClassName<SpaceManager>();
	auto& settings = Engine::Settings::Instance();

	if (auto* spaceManager = settings.JsonData(className, true)) {
		Space& space = Current();

		(*spaceManager)[currentSpaceKey] = space.GetName();

		auto& generateData = (*spaceManager)["generateData"];
		generateData["type"] = space.typeTemp;
		generateData["count"] = space.countTemp;
		generateData["size"] = space.sizeTemp;
	}
}
