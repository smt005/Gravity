// ◦ Xyz ◦

#include "Settings.h"
#include "FileManager.h"

using namespace Engine;

nlohmann::json& Settings::LoadSettings()
{
	try {
		nlohmann::json jsonData = Engine::LoadJson(std::string(settingsFileName), "write");

		if (jsonData.contains("width")) {
			width = jsonData["width"].get<int>();
		}
		if (jsonData.contains("height")) {
			height = jsonData["height"].get<int>();
		}
		if (jsonData.contains("top")) {
			top = jsonData["top"].get<int>();
		}
		if (jsonData.contains("left")) {
			left = jsonData["left"].get<int>();
		}
		if (jsonData.contains("fullScreen")) {
			fullScreen = jsonData["fullScreen"].get<int>();
		}

		std::swap(_jsonData, jsonData);
	}
	catch (...) {
		LOG("[Core::LoadSettings] fail load settings.");
	}

	return _jsonData;
}

void Settings::SaveSettings()
{
	Engine::SaveJson(_jsonData, std::string(settingsFileName), "write");
}

nlohmann::json& Settings::JsonData()
{
	return _jsonData;
}

const nlohmann::json& Settings::JsonData() const
{
	return _jsonData;
}

const nlohmann::json* Settings::JsonData(std::string_view path) const
{
	const nlohmann::json* jsonData = &_jsonData;

	for (std::string_view key : mystd::SplitString(path, '/')) {
		if (jsonData->contains(key)) {
			jsonData = &(*jsonData)[key];
		}
		else {
			return nullptr;
		}
	}

	return jsonData;
}


nlohmann::json* Settings::JsonData(std::string_view path, bool create)
{
	nlohmann::json* jsonData = &_jsonData;

	for (std::string_view key : mystd::SplitString(path, '/')) {
		if (jsonData->contains(key)) {
			jsonData = &(*jsonData)[key];
		}
		else if (create) {
			(*jsonData)[key] = nlohmann::json();
			jsonData = &(*jsonData)[key];
		}
		else {
			return nullptr;
		}
	}

	return jsonData;
}

nlohmann::json Settings::operator [](std::string_view keys)
{
	auto* jsonData = JsonData(keys);
	return jsonData ? *jsonData : nlohmann::json();
}
