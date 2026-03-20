// ◦ Xyz ◦
#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <Log.h>

namespace Engine
{
	template <typename T>
	T GetJsonValue(std::string_view key, const nlohmann::json& jsonData)
	{
		if (!jsonData.contains(key)) {
			return {};
		}

		try {
			return jsonData[key].get<T>();
		}
		catch (const std::exception& exc) {
			LOG("[Engine::GetJsonValue] key: '{}', '{}', type: '{}' , ", key, exc.what(), typeid(T).name());
		}
		catch (...) {
			LOG("[Engine::GetJsonValue] key: '{}', type: '{}' , ", key, typeid(T).name());
		}

		return {};
	}

	template <typename T>
	T GetJsonValue(std::string_view key, const nlohmann::json* jsonData)
	{
		return jsonData ? GetJsonValue<T>(key, *jsonData) : T();
	}
}
