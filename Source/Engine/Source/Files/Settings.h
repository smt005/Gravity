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
		nlohmann::json* JsonData(std::string_view path, bool create = false);

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
