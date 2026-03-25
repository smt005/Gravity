// ◦ Xyz ◦
#pragma once

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
	static nlohmann::json& GetSettingData(std::string_view path, bool create = false);
	static void StopUpdate();
	static void CheckOverload(double deltaTime);
	static void CollectDebugData();

	static glm::vec3 CenteMassSpace();
	//static void AddBodyOnOrbit(const glm::vec3& pos, const Body& mainBody);
	static glm::vec3 GetVelocityOnOrbit(Body& body, const Body& mainBody);
	static void GenerateBox(int count, float minSpaceRange, float spaceRange);
	static void GeneratePlaneSphere(int count, float minSpaceRange, float spaceRange);
	static void GenerateOnOrbitSphere(int count, float minSpaceRange, float spaceRange);
	static void GetBodyPositions(std::vector<glm::vec3>& bodiesVec3);

	template <typename ... Args>
	static void Generate(GenerateType type, Args...args)
	{
		switch (type) {
		case GenerateType::BOX: GenerateBox(std::forward<Args>(args)...); break;
		case GenerateType::PLANE_SPHERE: GeneratePlaneSphere(std::forward<Args>(args)...); break;
		case GenerateType::ORBIT_ON_MAIN_BODT: GenerateOnOrbitSphere(std::forward<Args>(args)...); break;
		default: GenerateBox(std::forward<Args>(args)...);
		}
	}

	static glm::vec3 PosOfMinSpeedObject();
	static glm::vec3 PosOfMaxSpeedObject();
	static glm::vec3 PosOfMinMassObject();
	static glm::vec3 PosOfMaxMassObject();

//private: // TODO:
public:
	inline static int offsetIteration = 1; // 1000;
	inline static int countOfIteration = 1;
	inline static bool collapseBodies = true;

	inline static mystd::shared_ptr<Space> currentSpace;
	inline static std::vector<mystd::shared_ptr<Space>> spaces;
	inline static const std::string_view currentSpaceKey = "currentSpace";
};
