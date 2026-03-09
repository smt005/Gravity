// ◦ Xyz ◦
#pragma once

#include <Draw/Mesh.h>
#include <Object/Resource.h>

namespace Engine
{
	class Shape final : public Resource<Shape>
	{
	public:
		using Ptr = mystd::shared_ptr<Shape>;

		Shape() = default;
		Shape(std::string_view name, bool load = true, bool bindVao = false);
		Shape(std::string_view name, std::string_view fileNamePath, bool load = false, bool bindVao = false);
		~Shape() = default;

		bool Load(bool createBuffer = false);
		bool Load(std::string_view fileNamePath, bool createBuffer = false);

	private:
	public:
		Mesh mesh;
	};
}

#define SHAPES Engine::Shape::Instance()
