// ◦ Xyz ◦
#pragma once

#include <string>
#include <random>
//#include <corecrt_math_defines.h>

namespace Engine
{
	template<typename T>
	std::string GetClassName()
	{
		const std::string_view typeName = typeid(T).name();
		const size_t pos = typeName.find_last_of("::") + 1;

		if (pos != typeName.npos) {
			return std::string(typeName.substr(pos, typeName.size() - pos));
		}

		return std::string(typeName);
	}

	template<typename T>
	std::string GetClassName(T*)
	{
		return GetClassName<T>();
	}

	template <typename T>
	T Random(T min, T max)
	{
		static std::mt19937 rng{ std::random_device{}() };
		std::uniform_real_distribution<T> dist(min, max);
		return dist(rng);
	}

	template <typename Arg, typename ...Args>
	Arg&& FirstArg(Arg&& arg, Args&&...) {
		return std::forward<Arg>(arg);
	}
}
