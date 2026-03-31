// ◦ Xyz ◦

#include "Settings.h"
#include "FileManager.h"

using namespace Engine;

Json& Settings::LoadSettings()
{
	try {
		Json jsonData = Engine::LoadJson(std::string(settingsFileName), "write");

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

Json& Settings::JsonData()
{
	return _jsonData;
}

const Json& Settings::JsonData() const
{
	return _jsonData;
}

const Json* Settings::JsonData(std::string_view path) const
{
	const Json* jsonData = &_jsonData;

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


Json* Settings::JsonData(std::string_view path, bool create)
{
	Json* jsonData = &_jsonData;

	for (std::string_view key : mystd::SplitString(path, '/')) {
		if (jsonData->contains(key)) {
			jsonData = &(*jsonData)[key];
		}
		else if (create) {
			(*jsonData)[key] = Json();
			jsonData = &(*jsonData)[key];
		}
		else {
			return nullptr;
		}
	}

	return jsonData;
}

Json Settings::operator [](std::string_view keys)
{
	auto* jsonData = JsonData(keys);
	return jsonData ? *jsonData : Json();
}
