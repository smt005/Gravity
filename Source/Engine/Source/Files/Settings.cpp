// ◦ Xyz ◦

#include "Settings.h"
#include <vector>
#include "FileManager.h"

using namespace Engine;

nlohmann::json& Settings::LoadSettings()
{
	const std::string jsonText = Engine::FileManager::Get("write").ReadTextFile(Settings::settingsFileName);
	if (jsonText.empty()) {
		return _jsonData;
	}

	try {
		_jsonData = nlohmann::json::parse(jsonText);

		if (_jsonData.contains("width")) {
			width = _jsonData["width"].get<int>();
		}
		if (_jsonData.contains("height")) {
			height = _jsonData["height"].get<int>();
		}
		if (_jsonData.contains("top")) {
			top = _jsonData["top"].get<int>();
		}
		if (_jsonData.contains("left")) {
			left = _jsonData["left"].get<int>();
		}
		if (_jsonData.contains("fullScreen")) {
			fullScreen = _jsonData["fullScreen"].get<int>();
		}
	}
	catch (...) {
		LOG("[Core::LoadSettings] fail load settings.");
	}

	return _jsonData;
}

void Settings::SaveSettings()
{
	FileManager::Get("write").WriteFile(_jsonData.dump(2), Settings::settingsFileName);
}

nlohmann::json& Settings::JsonData()
{
	return _jsonData;
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
