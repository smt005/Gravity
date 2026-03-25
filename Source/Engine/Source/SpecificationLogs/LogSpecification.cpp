// ◦ Xyz ◦

#include "LogSpecification.h"

namespace glm
{

std::ostream& operator<<(std::ostream& os, mat4x4 const& vec)
{
	os << "\n|" << vec[0][0] << ",\t" << vec[0][1] << ",\t" << vec[0][2] << ",\t" << vec[0][3] << '|';
	os << "\n|" << vec[1][0] << ",\t" << vec[1][1] << ",\t" << vec[1][2] << ",\t" << vec[1][3] << '|';
	os << "\n|" << vec[2][0] << ",\t" << vec[2][1] << ",\t" << vec[2][2] << ",\t" << vec[2][3] << '|';
	os << "\n|" << vec[3][0] << ",\t" << vec[3][1] << ",\t" << vec[3][2] << ",\t" << vec[3][3] << '|';
	return os;
}

std::ostream& operator<<(std::ostream& os, vec4 const& vec)
{
	os << '[' << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ']';
	return os;
}

std::ostream& operator<<(std::ostream& os, vec3 const& vec)
{
	os << '[' << vec.x << ", " << vec.y << ", " << vec.z << ']';
	return os;
}

std::ostream& operator<<(std::ostream& os, vec2 const& vec)
{
	os << '[' << vec.x << ", " << vec.y << ']';
	return os;
}

} // namespace glm
