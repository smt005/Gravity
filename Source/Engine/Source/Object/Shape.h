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
		Shape(std::string_view name, bool load = true, bool bindVao = false);
		Shape(std::string_view name, std::string_view fileNamePath, bool load = false, bool bindVao = false);

		std::string_view Name() const {
			return _name;
		}

		bool Load(bool createBuffer = false);
		bool Load(std::string_view fileNamePath, bool createBuffer = false);

	private:
	public:
		std::string _name;
		Mesh mesh;
	};
}

#define SHAPES Engine::Shape::Instance()