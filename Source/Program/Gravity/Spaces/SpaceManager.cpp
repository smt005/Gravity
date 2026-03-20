// ◦ Xyz ◦

#include "SpaceManager.h"
#include "Space.h"
#include "OneThreadSpace.h"
#include <StringUtils.h>
#include <Files/Settings.h>
#include <Common/Common.h>
#include <Common/JsonHelper.h>

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
	const auto& settings = Engine::Settings::Instance();
	auto* jsonData = settings.JsonData(Engine::GetClassName<SpaceManager>());
	if (!jsonData) {
		currentPtr = SetCurrentPtr<OneThreadSpace>();
		currentPtr->Generate(100, 100, 0);
		return;
	}

	std::string currentSpaceName = Engine::GetJsonValue<std::string>(currentSpaceKey, jsonData);
	if (!currentSpaceName.empty()) {
		if (currentSpaceName == Engine::GetClassName<Space>()) {
			currentPtr = SetCurrentPtr<Space>();
		} else if (currentSpaceName == Engine::GetClassName<OneThreadSpace>()) {
			currentPtr = SetCurrentPtr<OneThreadSpace>();
		}
	}
	if (currentPtr) {
		if (jsonData->contains("generateData")) {
			auto& generateData = (*jsonData)["generateData"];
			int type = Engine::GetJsonValue<int>("type", generateData);
			int count = Engine::GetJsonValue<int>("count", generateData);
			int size = Engine::GetJsonValue<int>("size", generateData);

			if (type < 0) {
				type = 0;
			}
			if (count <= 0) {
				count = 100;
			}
			if (size <= 0) {
				size = 100;
			}

			currentPtr->Generate(count, size, type);
		}
	}
}

void SpaceManager::Save()
{
	auto& settings = Engine::Settings::Instance();
	if (auto* spaceManager = settings.JsonData(Engine::GetClassName<SpaceManager>(), true)) {
		Space& space = Current();

		(*spaceManager)[currentSpaceKey] = space.GetName();

		auto& generateData = (*spaceManager)["generateData"];
		generateData["type"] = space.typeTemp;
		generateData["count"] = space.countTemp;
		generateData["size"] = space.sizeTemp;
	}
}
