// ◦ Xyz ◦
#pragma once

#include <string>
#include <random>

namespace Engine
{
	static std::string ExtractClassName(const std::string_view fullName)
	{
		std::string result;
		size_t pos = fullName.npos;

		// TODO:
		{
			const std::string_view text = "::";
			pos = fullName.find_last_of(text);
			if (pos != fullName.npos) {
				pos += text.length();
			}
		}

		if (pos == fullName.npos) {
			const std::string_view text = "class ";
			pos = fullName.find(text);

			if (pos != fullName.npos) {
				pos += text.length();
			}
		}

		if (pos == fullName.npos) {
			const std::string_view text = "struct ";
			pos = fullName.find(text);

			if (pos != fullName.npos) {
				pos += text.length();
			}
		}

		if (pos != fullName.npos) {
			result = std::string(fullName.substr(pos, fullName.size() - pos));
		}
		else {
			result = std::string(fullName);
		}

		return result;
	}

	template <typename T>
	std::string GetClassName()
	{
		const std::string_view typeName = typeid(T).name();
		return ExtractClassName(typeName);
	}

	template <typename T>
	T Random(T min, T max)
	{
		static std::mt19937 rng{ std::random_device{}() };
		std::uniform_real_distribution<T> dist(min, max);
		return dist(rng);
	}

	template <typename Arg, typename ...Args>
	Arg&& FirstArg(Arg&& arg, Args&&...) {
		return std::forward<Arg>(arg);
	}
}
