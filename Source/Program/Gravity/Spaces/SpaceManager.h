// ◦ Xyz ◦
#pragma once

#include <atomic>
#include <vector>
#include <glm/vec3.hpp>
#include <mystd_memory.h>
#include "Space.h"
#include <StringUtils.h>
#include <Files/Settings.h>
#include <Common/JsonHelper.h>
#include <Common/Common.h>

class SpaceManager final
{
public:
	enum class GenerateType
	{
		BOX,
		PLANE_SPHERE,
		ORBIT_ON_MAIN_BODT,
		NONE
	};

	template<typename T>
	static const Space::Ptr& GetPtr(bool setCurrent = false)
	{
		for (const Space::Ptr& space : spaces) {
			if (typeid(*space.get()) == typeid(T)) {
				if (setCurrent) {
					currentSpace = space;
				}
				return space;
			}
		}

		// TODO: Exception
		auto space = mystd::make_shared<T>();
		if (setCurrent) {
			currentSpace = space;
		}

		return spaces.emplace_back(space);
	}

	template<typename T>
	static Space& Get(bool setCurrent = false)
	{
		return *GetPtr<T>(setCurrent);
	}

	template<typename T>
	static const Space::Ptr& SetCurrentPtr()
	{
		return GetPtr<T>(true);
	}

	template<typename T>
	static Space& SetCurrent()
	{
		return *SetCurrentPtr<T>();
	}

	template<typename T>
	static T GetGenerateValue(std::string_view key, T&& defValue) {
		const std::string keys = TO_STRING("{}/generationData/{}", Engine::GetClassName<SpaceManager>(), key);
		return Engine::Settings::Instance()[keys, std::forward<T>(defValue)];
	}

	static Space& Current();
	static const Space::Ptr& CurrentPtr();
	static void Update(double deltaTime);
	static void Load();
	static void Save();
	static bool LoadSpace();
	static void SaveSpace(bool addTimeToName = false);

	static Json& GetSettingData(std::string_view path = "", bool create = false);
	static void StopUpdate();
	static void CheckOverload(double deltaTime);
	static void CollectDebugData();
	static void GetBodyPositions(std::vector<float>& positions);
	static void GetBodyPositions(std::vector<glm::vec3>& positions);

	static glm::vec3 CenteMassSpace();

	template <typename TGenerator>
	static void GenerateSpace() {
		auto& space = Current();
		space.Clear();
		space.AddBodies(TGenerator::Generate());
	};

	template <typename T>
	static void SwitchSpace() {
		std::vector<BodyData> bodies = SpaceManager::Current().GetBodies();
		auto& newSpace = SpaceManager::SetCurrent<T>();
		newSpace.Clear();
		newSpace.AddBodies(bodies);
	}

//private: // TODO:
public:
	inline static std::atomic<float> offsetIteration = 1.f;
	inline static std::atomic<float> countOfIteration = 1.f;
	inline static std::atomic<bool> paramA = false;
	inline static std::atomic<bool> paramB = false;
	inline static std::atomic<bool> paramC = false;

	inline static std::vector<BodyData> bodies;
	inline static mystd::shared_ptr<Space> currentSpace;
	inline static std::vector<mystd::shared_ptr<Space>> spaces;
	inline static const std::string_view currentSpaceKey = "currentSpace";
};
