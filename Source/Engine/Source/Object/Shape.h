// ◦ Xyz ◦
#pragma once

#include <Draw/Mesh.h>
#include <Common/Storage.h>

namespace Engine
{
	class Shape final : public Storage<Shape>
	{
	public:
		Shape() = delete;
		Shape(std::string_view name)
			: _name(name)
		{
			LOG("Shape: {}, {}", name, _name);
		}

		std::string_view Name() const {
			return _name;
		}

	private:
		std::string _name;
		Mesh mesh;
	};
}
