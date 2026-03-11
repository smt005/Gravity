// ◦ Xyz ◦
#pragma once

#include <StringUtils.h>

namespace mystd
{
	void WriteLog(const std::string& text);

	template <typename... Args>
	void Print(Args&&... args) {
		const std::string text = ToString(std::forward<Args>(args)...);
		std::cout << text << std::endl;

#ifdef _DEBUG
		_CrtDbgReport(_CRT_WARN, NULL, 0, NULL, "%s\n", text.c_str());
#endif
	}

	template <typename... Args>
	std::string Log(Args&&... args) {
		const std::string text = ToString(std::forward<Args>(args)...);
		std::cout << "LOG:" << text << std::endl;
		WriteLog(text);
		return text;
	}

	template <typename... Args>
	std::string LogBreak(Args&&... args) {
		const std::string text = Log(std::forward<Args>(args)...);
		__debugbreak();
		return text;
	}
}

#define LOG mystd::Log
#define LOG_BREAK mystd::LogBreak
