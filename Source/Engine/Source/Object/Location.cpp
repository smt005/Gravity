// ◦ Xyz ◦

#include "Location.h"
#include <glm/ext/matrix_transform.hpp>

#include <Common/Common.h>

using namespace Engine;

Location::Location(std::string_view name, bool needLoad)
	: Resource(name)
{
	if (_objects.empty() && needLoad) {
		Load();
	}
}

void Location::Load()
{
	constexpr size_t sizeLocation = 100;
	constexpr float sizePlane = 100.f;
	constexpr float offset = static_cast<float>(sizeLocation) / 2.f * sizePlane / 2.f;
	constexpr size_t countModeNames = 5;
	constexpr std::string_view modelNames[] = { "Plane_100", "Plane_12_100", "Plane_25_100", "Plane_50_100", "Plane_100_100" };

	_objects.reserve(sizeLocation * sizeLocation);

	for (size_t iX = 0; iX < sizeLocation; ++iX) {
		for (size_t iY = 0; iY < sizeLocation; ++iY) {
			size_t indexModelName = std::rand() % countModeNames;
			
			if (std::rand() % 3 > 0) {
				indexModelName = 0;
			}

			if (indexModelName >= countModeNames) {
				//PRINT("rand: {} ERROR", indexModelName);
				indexModelName = 0;
			}
			else {
				//PRINT("rand: {}", indexModelName);
			}

			const std::string_view nameModel = modelNames[indexModelName];
	
			_objects.emplace_back().model = MODELS.Get(nameModel, true, true);
			glm::mat mat = glm::translate(_objects.back().mat, glm::vec3((static_cast<float>(iX) * sizePlane - offset), (static_cast<float>(iY) * sizePlane - offset), 0.f));
			_objects.back().mat = mat;
		}
	}

	{
		_gliders.reserve(4);

		{
			_gliders.emplace_back().model = MODELS.Get("Glider00", true, true);
			_gliders.back().mat[3][0] += 200;
			_gliders.back().mat[3][1] += 200;
			_gliders.back().mat[3][2] += 10;
		}
		{
			_gliders.emplace_back().model = MODELS.Get("Glider00", true, true);
			_gliders.back().mat[3][0] += 300;
			_gliders.back().mat[3][1] += 400;
			_gliders.back().mat[3][2] += 10;
		}
		{
			_gliders.emplace_back().model = MODELS.Get("Glider01", true, true);
			_gliders.back().mat[3][0] -= 200;
			_gliders.back().mat[3][1] -= 200;
			_gliders.back().mat[3][2] += 10;
		}
		{
			_gliders.emplace_back().model = MODELS.Get("Glider01", true, true);
			_gliders.back().mat[3][0] -= 300;
			_gliders.back().mat[3][1] -= 400;
			_gliders.back().mat[3][2] += 10;
		}
	}

}
