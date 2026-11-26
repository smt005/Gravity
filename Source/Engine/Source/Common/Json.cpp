// ◦ Xyz ◦
#include "Json.h"
#include <Common/JsonHelper.h>

using namespace Engine;

JsonRef::operator const nlohmann::json& () const
{
	if (_jsonRef.empty() || !(_jsonRef.is_array() || _jsonRef.is_object())) {
		LOG_BREAK("[Engine::JsonRef] const nlohmann::json& () fail");
		static const nlohmann::json jsonTemp;
		return jsonTemp;
	}
	return _jsonRef;
}

JsonRef::operator int() const
{
	if (_jsonRef.empty() || !_jsonRef.is_number_integer()) {
		LOG_BREAK("[Engine::JsonRef] int () fail");
		return {};
	}
	return _jsonRef.get<int>();
}

JsonRef::operator float() const
{
	if (_jsonRef.empty() || !_jsonRef.is_number_float()) {
		LOG_BREAK("[Engine::JsonRef] float () fail");
		return {};
	}
	return _jsonRef.get<float>();
}

JsonRef::operator std::string() const
{
	if (_jsonRef.empty() || !_jsonRef.is_string()) {
		LOG_BREAK("[Engine::JsonRef] std::string () fail");
		return {};
	}
	return _jsonRef.get<std::string>();
}

JsonRef::operator bool() const
{
	if (_jsonRef.empty() || !_jsonRef.is_boolean()) {
		LOG_BREAK("[Engine::JsonRef] bool () fail");
		return {};
	}
	return _jsonRef.get<bool>();
}

JsonRef::operator long long() const
{
	if (_jsonRef.empty() || !_jsonRef.is_number()) {
		LOG_BREAK("[Engine::JsonRef] long long () fail");
		return {};
	}
	return _jsonRef.get<long long>();
}

JsonRef::operator size_t() const
{
	if (_jsonRef.empty() || !_jsonRef.is_number_unsigned()) {
		LOG_BREAK("[Engine::JsonRef] size_t () fail");
		return {};
	}
	return _jsonRef.get<size_t>();
}


Json::Json(const std::filesystem::path& fileNamePath, const std::string& fileSystemName)
	: _jsonData(Engine::LoadJson(fileNamePath, fileSystemName))
{}

Json::Json(const nlohmann::json& jsonData)
	: _jsonData(jsonData)
{}

Json::Json(const JsonRef& other)
	: _jsonData(other._jsonRef)
{}

Json::Json(const Json& other)
	: _jsonData(other._jsonData)
{}

Json::Json(Json&& other) noexcept
{
	std::swap(_jsonData, other._jsonData);
}

Json& Json::operator = (const JsonRef& other)
{
	_jsonData = other._jsonRef;
	return *this;
}

Json& Json::operator = (const Json& other)
{
	if (this != &other) {
		_jsonData = other._jsonData;
	}
	return *this;
}

Json& Json::operator = (Json&& other) noexcept
{
	if (this != &other) {
		std::swap(_jsonData, other._jsonData);
	}
	return *this;
}
