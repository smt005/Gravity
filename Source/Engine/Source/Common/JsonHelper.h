// ◦ Xyz ◦
#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <Logs.h>

namespace Engine
{
	template <typename T>
	T GetJsonValue(std::string_view key, const nlohmann::json& jsonData, const T& defaultValue = T())
	{
		if (!jsonData.contains(key)) {
			return defaultValue;
		}

		try {
			return jsonData.at(key).get<T>();
		}
		catch (const std::exception& exc) {
			LOG("[Engine::GetJsonValue] key: '{}', '{}', type: '{}' , ", key, exc.what(), typeid(T).name());
		}
		catch (...) {
			LOG("[Engine::GetJsonValue] key: '{}', type: '{}' , ", key, typeid(T).name());
		}

		return defaultValue;
	}

	template <typename T>
	T GetJsonValue(std::string_view key, const nlohmann::json* jsonData, const T& defaultValue = T())
	{
		return jsonData ? GetJsonValue<T>(key, *jsonData, defaultValue) : defaultValue;
	}
}
