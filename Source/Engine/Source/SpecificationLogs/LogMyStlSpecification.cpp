// ◦ Xyz ◦

#include "LogMyStlSpecification.h"

namespace mystd
{
	std::ostream& operator<<(std::ostream& os, const Vec3& vec)
	{
		os << '[' << vec.X() << ", " << vec.Y() << ", " << vec.Z() << ']';
		return os;
	}
}
