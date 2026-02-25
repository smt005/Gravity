// ◦ Xyz ◦
#pragma once


#include <iostream>
#include <sstream>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <FileManager/FileManager.h>

namespace mystd
{

	std::ostream& operator << (std::ostream& os, const glm::vec4& vec)
	{
		os << '[' << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ']';
		return os;
	}

	std::ostream& operator << (std::ostream& os, const glm::vec3& vec)
	{
		os << '[' << vec.x << ", " << vec.y << ", " << vec.z << ']';
		return os;
	}

	std::ostream& operator << (std::ostream& os, const glm::vec2& vec)
	{
		os << '[' << vec.x << ", " << vec.y << ']';
		return os;
	}

	std::ostream& operator << (std::ostream& os, const glm::mat4x4& vec)
	{
		os << "\n|" << vec[0][0] << ",\t" << vec[0][1] << ",\t" << vec[0][2] << ",\t" << vec[0][3] << '|';
		os << "\n|" << vec[1][0] << ",\t" << vec[1][1] << ",\t" << vec[1][2] << ",\t" << vec[1][3] << '|';
		os << "\n|" << vec[2][0] << ",\t" << vec[2][1] << ",\t" << vec[2][2] << ",\t" << vec[2][3] << '|';
		os << "\n|" << vec[3][0] << ",\t" << vec[3][1] << ",\t" << vec[3][2] << ",\t" << vec[3][3] << '|';
		return os;
	}

	std::ostream& operator << (std::ostream& os, const Engine::FileManager& fm)
	{
		os << "'" << fm.GetName() << "'\n" << fm.GetRoot();
		return os;
	}
}
