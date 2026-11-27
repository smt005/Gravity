// ◦ Xyz ◦
#pragma once

//
#include <any>
#include <variant>
#include <memory>
#include <string_view>
#include <type_traits>

//#include <string>
//#include "Log.h"

namespace mystd
{
	using AnyData = std::unique_ptr<std::any>;
	using Variant = std::variant<
		bool,
		char,
		int,
		unsigned int,
		float,
		double,
		long long,
		size_t,
		AnyData>;

	class MyJson
	{
	public:
		MyJson() = default;
		MyJson(std::string_view dataStr);

		void Parse(std::string_view dataStr);

		template <typename T>
		MyJson& operator = (T&& data)
		{
			if constexpr (std::is_fundamental_v<std::remove_cvref_t<T>>) {
				_data = data;
			}
			else {
				std::any anyData = std::forward<T>(data);
				_data = std::make_unique<std::any>(std::move(anyData));
			}

			return *this;
		}

		template <typename T>
		operator T() const
		{
			if constexpr (std::is_fundamental_v<std::remove_cvref_t<T>>) {
				return std::get<T>(_data);
			}

			const AnyData& anyDataPtr = std::get<AnyData>(_data);
			const std::any& anyData = *anyDataPtr;
			return std::any_cast<T>(anyData);
		}
	private:
		Variant _data;
	};

	void TestJsonMystd();
}
