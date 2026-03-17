// ◦ Xyz ◦
#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace mystd
{
	template <typename Arg>
	void ToStreamArgs(std::ostringstream& oss, Arg&& arg) {
		oss << arg;
	}

	template <typename Arg>
	void ToStreamArgs(std::ostringstream& oss, std::string_view&& format, Arg&& arg) {
		const size_t pos = format.find("{}");
		if (pos == format.npos) {
			oss << format << arg;
			return;
		}
		oss << format.substr(0, pos) << arg << format.substr(pos + 2, format.size() - pos);
	}

	template <typename Arg, typename... Args>
	void ToStreamArgs(std::ostringstream& oss, std::string_view&& format, Arg&& arg, Args&&... args) {
		const size_t pos = format.find("{}");
		if (pos == format.npos) {
			oss << format << arg;
			ToStreamArgs(oss, "", std::forward<Args>(args)...);
			return;
		}
		oss << format.substr(0, pos) << arg;
		ToStreamArgs(oss, format.substr(pos + 2, format.size() - pos), std::forward<Args>(args)...);
	}

	template <typename... Args>
	std::string ToString(Args&&... args) {
		std::ostringstream oss;
		ToStreamArgs(oss, std::forward<Args>(args)...);
		return oss.str();
	}

	static std::vector<std::string_view> SplitString(std::string_view str, char devider) {
		std::vector<std::string_view> parsedStr;
		size_t pos = 0;
		size_t posLast = str.find(devider);

		while (posLast != str.npos) {
			parsedStr.emplace_back(str.substr(pos, posLast - pos));
			pos = posLast + 1;
			posLast = str.find(devider, pos);
		}

		if (pos != str.npos) {
			parsedStr.emplace_back(str.substr(pos, str.size() - pos));
		}

		return parsedStr;
	}
}

#define TO_STRING mystd::ToString
