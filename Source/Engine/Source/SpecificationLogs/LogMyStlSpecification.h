// ◦ Xyz ◦
#pragma once

#include <iostream>
#include <mystd_memory.h>
#include <MyMath.h>

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

	std::ostream& operator<<(std::ostream& os, const Vec3& v);
}
