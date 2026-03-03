// ◦ Xyz ◦

#include "LogSpecification.h"

std::ostream& operator << (std::ostream& os, const glm::mat4x4& vec)
{
	os << "\n|" << vec[0][0] << ",\t" << vec[0][1] << ",\t" << vec[0][2] << ",\t" << vec[0][3] << '|';
	os << "\n|" << vec[1][0] << ",\t" << vec[1][1] << ",\t" << vec[1][2] << ",\t" << vec[1][3] << '|';
	os << "\n|" << vec[2][0] << ",\t" << vec[2][1] << ",\t" << vec[2][2] << ",\t" << vec[2][3] << '|';
	os << "\n|" << vec[3][0] << ",\t" << vec[3][1] << ",\t" << vec[3][2] << ",\t" << vec[3][3] << '|';
	return os;
}

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

/*std::ostream& operator << (std::ostream& os, const Engine::Mesh& mesh)
{
	{
		const size_t size = mesh.CountVertexes();
		os << '[';

		for (int i = 0; i < size; ++i) {
			try {
				float value = mesh.Data()[i];
				os << value << ',';
			}
			catch (...) {
				os << "VERTEX STOP i: " << i << " / " << size;
			}
		}

		os << ']';
	}
	{
		const size_t size = mesh.SizeData();
		os << '[';

		for (int i = 0; i < size; ++i) {
			try {
				float value = mesh.Data()[i];
				os << value << ',';
			}
			catch (...) {
				os << "DATA STOP i: " << i << " / " << size;
			}
		}

		os << ']';
	}

	return os;
}*/
