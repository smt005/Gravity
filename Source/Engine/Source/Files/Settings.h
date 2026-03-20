// ◦ Xyz ◦
#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <Singletone.h>

namespace Engine
{
	class Settings : public mystd::Singletone<Settings>
	{
	public:
		nlohmann::json& LoadSettings();
		void SaveSettings();
		nlohmann::json& JsonData();
		const nlohmann::json& JsonData() const;
		const nlohmann::json* JsonData(std::string_view path) const;
		nlohmann::json* JsonData(std::string_view path, bool create);

	public:
		int left = 100;
		int top = 100;
		int width = 400;
		int height = 200;
		bool fullScreen = false; // TODO:

	private:
		nlohmann::json _jsonData;

	private:
		inline static const std::string_view settingsFileName = "Settings.json";
	};
}
