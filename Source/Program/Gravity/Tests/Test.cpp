// ◦ Xyz ◦

#include "Test.h"
#include <vector>
#include <MyMath.h>
#include <glm/vec3.hpp>
#include <Common/Common.h>
#include <Logs.h>

namespace
{
	bool CheckVectors(const glm::vec3 glmVec3, const mystd::Vec3& myVec3)
	{
		if (glmVec3.x == myVec3.X() && glmVec3.y == myVec3.Y() && glmVec3.z == myVec3.Z()) {
			return true;
		}

		return false;
	}
}

void Tests::FunVec3()
{
	const size_t size = 1000;

	std::vector<glm::vec3> glmVectors;
	std::vector<mystd::Vec3> myVectors;

	for (size_t i = 0; i < size; ++i) {
		const float x = Engine::Random(-1000000.f, 1000000.f);
		const float y = Engine::Random(-1000000.f, 1000000.f);
		const float z = Engine::Random(-1000000.f, 1000000.f);

		glmVectors.emplace_back(x, y, z);
		myVectors.emplace_back(x, y, z);
	}

	/*for (size_t i = 0; i < size; ++i) {
		PRINT("{}: {} - {}", i, glmVectors[i], mystdVectors[i]);

		if (!CheckVectors(glmVectors[i], myVectors[i])) {
			PRINT("FAIL: {}", i);
			__debugbreak();
		}
	}*/

	{
		for (size_t i = 0; i < size; ++i) {
			const auto glmVec3 = glm::normalize(glmVectors[i]);
			auto myVec3 = myVectors[i];
			myVec3.Normalized();

			PRINT("{}: {} - {}", i, glmVectors[i], myVectors[i]);

			if (!CheckVectors(glmVectors[i], myVectors[i])) {
				PRINT("FAIL: {}", i);
				__debugbreak();
			}
		}
	}

	__debugbreak();
}
