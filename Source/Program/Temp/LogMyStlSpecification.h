// ◦ Xyz ◦
#pragma once

#include <iostream>
#include <sstream>
#include <mystd_memory.h>
#include <Log.h>

namespace mystd
{
	template <typename T>
	std::ostream& operator << (std::ostream& os, const simple_shared_ptr<T>& sPtr)
	{
		if (!sPtr) {
			os << "null";
			return os;
		}
		os << *sPtr;
		return os;
	};
}
