// ◦ Xyz ◦
#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <StringUtils.h>

namespace Engine
{
	class Json;

	class JsonRef final
	{
		friend Json;

	public:
		JsonRef(const nlohmann::json& jsonRef)
			: _jsonRef(jsonRef)
		{}

		operator const nlohmann::json& () const;
		operator int() const;
		operator float() const;
		operator std::string() const;
		operator bool() const;
		operator long long() const;
		operator size_t() const;

	private:
		const nlohmann::json& _jsonRef;
	};

	class Json final
	{
	public:
		Json(const std::filesystem::path& fileNamePath, const std::string& fileSystemName = "base");
		Json(const nlohmann::json& jsonData);
		Json(const JsonRef& other);
		Json(const Json& other);
		Json(Json&& other) noexcept;
		Json& operator = (const JsonRef& other);
		Json& operator = (const Json& other);
		Json& operator = (Json&& other) noexcept;

		template <typename ...Args>
		JsonRef operator [](Args&&... args) const
		{
			std::vector<std::string> keys = CollectKeys(std::forward<Args>(args)...);
			const nlohmann::json* jsonData = &_jsonData;

			for (const std::string& key : keys) {
				jsonData = &jsonData->at(key);
			}

			return { *jsonData };
		}

		template <typename ...Args>
		JsonRef At(Args&&... args) const
		{
			std::vector<std::string> keys = CollectKeys(std::forward<Args>(args)...);
			const nlohmann::json* jsonData = &_jsonData;

			for (const std::string& key : keys) {
				jsonData = &jsonData->at(key);
			}

			return { *jsonData };
		}

	private:
		template <typename Arg>
		void DivideKeyArgumets(std::vector<std::string>& keys, Arg&& arg) const
		{
			std::ostringstream oss;
			oss << arg;
			const std::string keysStr = oss.str();

			for (std::string_view key : mystd::SplitString(keysStr, '/')) {
				keys.emplace_back(key);
			}
		}

		template <typename Arg, typename ...Args>
		void DivideKeyArgumets(std::vector<std::string>& keys, Arg&& arg, Args&&... args) const
		{
			std::ostringstream oss;
			oss << arg;
			const std::string keysStr = oss.str();
			
			for (std::string_view key : mystd::SplitString(keysStr, '/')) {
				keys.emplace_back(key);
			}

			DivideKeyArgumets(keys, std::forward<Args>(args)...);
		}

		template <typename ...Args>
		std::vector<std::string> CollectKeys(Args&&... args) const
		{
			std::vector<std::string> keys;
			DivideKeyArgumets(keys, std::forward<Args>(args)...);
			return keys;
		}

	private:
		nlohmann::json _jsonData;
	};
}
