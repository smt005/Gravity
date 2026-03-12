// ◦ Xyz ◦
#pragma once

#include <vector>
#include <mystd_memory.h>
#include "Space.h"
#include "MainThreadSpace.h"

class SpaceManager final
{
public:
	template<typename T>
	static Space& Get(bool setCurrent = false)
	{
		for (const Space::Ptr& space : spaces) {
			if (typeid(space.get()) == typeid(T)) {
				return *space;
			}
		}

		// TODO: Exception
		//Space::Ptr space = mystd::make_shared<T>();
		auto space = mystd::make_shared<T>();

		if (setCurrent) {
			currentSpace = space;
		}

		spaces.emplace_back(space);
		return *space;
	}

	template<typename T>
	static Space& SetCurrent()
	{
		return Get<T>(true);
	}

	static Space& Current();
	static void Update();
	
private:
	inline static mystd::shared_ptr<Space> currentSpace;
	inline static std::vector<mystd::shared_ptr<Space>> spaces;
};
