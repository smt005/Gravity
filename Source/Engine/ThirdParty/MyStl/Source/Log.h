// ◦ Xyz ◦
#pragma once

//#include <iostream>
#include <sstream>
#include <string>
//#include <string_view>
#include <StringUtils.h>

namespace mystd
{
	template <typename... Args>
	void Log(Args&&... args) {
		const std::string text = ToString(std::forward<Args>(args)...);
		std::cout << "LOG:" << text << std::endl;

	#ifdef _DEBUG
		_CrtDbgReport(_CRT_WARN, NULL, 0, NULL, "LOG: %s\n", text.c_str());
	#endif // DEBUG
	}
}

#define LOG mystd::Log
