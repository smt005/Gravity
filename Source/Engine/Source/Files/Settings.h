// ◦ Xyz ◦
#pragma once

#include <string>
#include <Common/JsonHelper.h>
#include <Singletone.h>

namespace Engine
{
	class Settings : public mystd::Singletone<Settings>
	{
	public:
		Json& LoadSettings();
		void SaveSettings();
		Json& JsonData();
		const Json& JsonData() const;
		const Json* JsonData(std::string_view path) const;
		Json* JsonData(std::string_view path, bool create);

		Json operator [](std::string_view keys);

		template <typename T>
		Json operator [](std::string_view keys, T&& defValue) {
			auto* jsonValue = JsonData(keys);
			return jsonValue ? jsonValue->get<T>() : std::forward<T>(defValue);
		}

	public:
		int left = 100;
		int top = 100;
		int width = 400;
		int height = 200;
		bool fullScreen = false; // TODO:

	private:
		Json _jsonData;

	private:
		inline static const std::string_view settingsFileName = "Settings.json";
	};
}
