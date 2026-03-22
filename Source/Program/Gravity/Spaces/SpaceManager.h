// ◦ Xyz ◦
#pragma once

#include <vector>
#include <mystd_memory.h>
#include "Space.h"
#include "OneThreadSpace.h"
#include <StringUtils.h>
#include <Files/Settings.h>
#include <Common/JsonHelper.h>
#include <Common/Common.h>

class SpaceManager final
{
public:
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

		return spaces.emplace_back(space);;
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
		const std::string keys = TO_STRING("{}/generateData/{}", Engine::GetClassName<SpaceManager>(), key);
		return Engine::Settings::Instance()[keys, std::forward<T>(defValue)];
	}

	static Space& Current();
	static const Space::Ptr& CurrentPtr();
	static void Update();
	static void Load();
	static void Save();

private: // TODO:
public:
	inline static mystd::shared_ptr<Space> currentSpace;
	inline static std::vector<mystd::shared_ptr<Space>> spaces;
	inline static const std::string_view currentSpaceKey = "currentSpace";
};
