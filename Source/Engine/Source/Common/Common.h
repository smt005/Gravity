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

	/*float RandomFloat(float min, float max)
	{
		static std::mt19937 rng{ std::random_device{}() };
		std::uniform_real_distribution<float> dist(min, max);
		return dist(rng);
	}*/

	template <typename T>
	T Random(T min, T max)
	{
		static std::mt19937 rng{ std::random_device{}() };
		std::uniform_real_distribution<T> dist(min, max);
		return dist(rng);
	}

	/*template <typename T>
	int Random(T lowerBound, T upperBound) {
		if (lowerBound > upperBound) {
			std::swap(lowerBound, upperBound);
		}

		std::random_device rd; // Инициализация случайного устройства
		std::mt19937 gen(rd()); // Генератор с использованием устройства
		std::uniform_int_distribution<> distrib(lowerBound, upperBound); // Равномерное распределение

		return distrib(gen);
	}*/
}
