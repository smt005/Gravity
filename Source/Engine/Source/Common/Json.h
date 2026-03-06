// ◦ Xyz ◦
#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <Log.h>

namespace Engine
{
	template <typename T>
	T GetJsonValue(const std::string& key, nlohmann::json& jsonData)
	{
		try {
			return jsonData["width"].get<T>();
		}
		catch (const std::exception& exc) {
			LOG("[Engine::GetJsonValue] key: '{}', '{}', type: '{}' , ", key, exc.what(), typeid(T).name());
		}
		catch (...) {
			LOG("[Engine::GetJsonValue] key: '{}', type: '{}' , ", key, typeid(T).name());
		}

		return {};
	}
}
